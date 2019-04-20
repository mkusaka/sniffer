#include <stdexcept>

#include <boost/format.hpp>
#include <boost/program_options.hpp>

#include "arg_option.hpp"

namespace {
namespace bp = boost::program_options;
namespace bst = boost;

void create_options_description(bp::options_description& description) {
  description.add_options()
    ("help,h", "produce help message")
    ("lconfig,l", bp::value<std::string>(), 
     "specify logger's config file path")
    ("sconfig,s", bp::value<std::string>(),
     "specify sniffer's config file path");
}

void create_variables_map(bp::variables_map& var_map,
                          const bp::options_description description,
                          const int argc, const char** argv) {
  // parse argument options
  // throw bp::invalid_command_line_syntax on failure
  bp::store(bp::parse_command_line(argc, argv, description), var_map);
  bp::notify(var_map);
}

bool is_not_zero_length(const std::string value) {
  return value.length() > 0;
}

void validate_common(const bp::variables_map var_map, const std::string key) {
  // as<std::string>() throw bst::bad_any_cast on failure
  if (!var_map.count(key) ||
      !is_not_zero_length(var_map[key].as<std::string>())) {
    const auto msg = bst::format("%1% is invalid") % key;
    throw (std::invalid_argument(msg.str()));
  }
}

void validate(const bp::variables_map& var_map) {
  // throw bst::bad_any_cast or std::invalid_argument on failure
  validate_common(var_map, "lconfig");
  validate_common(var_map, "sconfig");
}
}   // namespace


ArgOption::ArgOption(const int argc, const char** argv) :
description_("command line options") {
  create_options_description(description_);

  // throw bp::invalid_command_line_syntax on failure
  create_variables_map(var_map_, description_, argc, argv);

  // if -h,--help option is specified,
  // return
  if (count("help")) {
    return;
  }

  // throw following exceptions on failure
  // - bst::bad_any_cast
  // - std::invalid_argument
  validate(var_map_);
}

bool ArgOption::count(const std::string key) {
  return var_map_.count(key);
}

void ArgOption::get(const std::string key, std::string& value) {
  value = var_map_[key].as<std::string>();
}

std::ostream& operator<<(std::ostream& out, ArgOption& argOption) {
  return out << argOption.description_;
}
