#include <iostream>   // for cerr
#include <dlfcn.h>    // dlopen, dlsym, dlclose
#include <stdexcept>
#include <string>
#include <unistd.h>   // for fork

#include <log4cpp/Category.hh>
#include <log4cpp/PropertyConfigurator.hh>

#include <arg_option.hpp>
#include <config.hpp>
#include <notifier.hpp>
#include <sniffer.hpp>

namespace {
void* open_handle(const std::string impl_path) {
  if (impl_path.length() <= 0) {
    throw std::invalid_argument("could not read implementation path");
  }

  void* handle;
  handle = dlopen(impl_path.c_str(), RTLD_LAZY);
  if (!handle) {
    throw std::runtime_error("could not dlopen implementation");
  }

  return handle;
}

Notifier* create_notifier(const Config config, void* handle) {
  Notifier* (*create)(const Config);

  create = (Notifier* (*)(const Config)) dlsym(
            handle, std::string("create").c_str());

  Notifier* impl = create(config);

  return impl;
}

Sniffer* create_sniffer(
  const Config config, std::shared_ptr<Notifier> notifier, void* handle) {
  Sniffer* (*create)(const Config, std::shared_ptr<Notifier> notifier);

  create = (Sniffer* (*)(const Config, std::shared_ptr<Notifier>)) dlsym(
            handle, std::string("create").c_str());

  Sniffer* impl = create(config, notifier);

  return impl;
}

void close_handle(void* handle) {
  dlclose(handle);
}
}   // namespace


int main (const int argc, const char** argv) {
  std::string log_config_path;
  std::string sniffer_config_path;

  // throw following exceptions on failure
  // - bp::invalid_command_line_syntax
  // - bst::bad_any_cast
  // - std::invalid_argument
  try {
    // parse each argument options
    ArgOption argOption(argc, argv);

    // if argOption instance gets -h,--help option,
    // provide help message
    if (argOption.count("help")) {
      std::cerr << argOption << std::endl;
      return EXIT_SUCCESS;
    }

    // get each values of argument options
    argOption.get("lconfig", log_config_path);
    argOption.get("sconfig", sniffer_config_path);
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
    std::cerr << "failed to parse argument options. please check -h,"
              << "--help option." << std::endl;
    return EXIT_FAILURE;
  }

  // setup logger
  // throw log4cpp::ConfigureFailure on failure
  try {
    log4cpp::PropertyConfigurator::configure(log_config_path);
  } catch (log4cpp::ConfigureFailure& e) {
    std::cerr << e.what() << std::endl;
    std::cerr << "failed to initialize logger" << std::endl;
    return EXIT_FAILURE;
  }

  log4cpp::Category& logger = log4cpp::Category::getInstance (__FILE__);
  logger.info("sniffer start");

  // read config file
  Config config(sniffer_config_path);

  int p_id;
  if ((p_id = fork()) == 0) {
    // create notifier
    void* notifier_lib_handle;
    void* sniffer_lib_handle;
    try {
      //
      // create notifier instance
      //
      // get notifier's implementation path
      std::string notifier_impl_path;
      config.get_notifier("implementation", notifier_impl_path);
      logger.debug("create notifier instance from " + notifier_impl_path);
      // create handle
      notifier_lib_handle = open_handle(notifier_impl_path);
      // get notifire instance
      std::shared_ptr<Notifier> notifier(
        create_notifier(config, notifier_lib_handle));

      //
      // create sniffer instance
      //
      // get sniffer's implementation path
      std::string sniffer_impl_path;
      config.get_sniffer("implementation", sniffer_impl_path);
      logger.debug("create sniffer instance from " + sniffer_impl_path);
      // create handle
      sniffer_lib_handle = open_handle(sniffer_impl_path);
      // get sniffer instance
      std::shared_ptr<Sniffer> sniffer(
        create_sniffer(config, notifier, sniffer_lib_handle));

      //
      // run sniffer
      //
      logger.info("run sniffer");
      sniffer->sniff();
    } catch (std::exception &e) {
      logger.error(e.what());
    }

    // close handle
    logger.info("close notifier and sniffer");
    close_handle(notifier_lib_handle);
  }

  logger.info("sniffer finish");
  return EXIT_SUCCESS;
}
