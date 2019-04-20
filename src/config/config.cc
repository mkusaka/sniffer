#include <stdexcept>
#include <string>

#include <boost/format.hpp>
#include <boost/optional.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/lexical_cast.hpp>

#include <log4cpp/Category.hh>

#include "config.hpp"

namespace {
namespace bpt = boost::property_tree;
namespace bst = boost;

log4cpp::Category& logger = log4cpp::Category::getInstance (__FILE__);

template<typename T>
void get(const std::string key,
         const bpt::ptree& property_tree, T& value) {
  logger.debug("get start");
  // get value from property_tree
  bst::optional<T> bst_value = property_tree.get_optional<T>(key);
  if (bst_value) {
    value = bst_value.get();
    std::string value_str = boost::lexical_cast<std::string>(value);
    logger.debug("get: " + key + " = " + value_str);
  } else {
    const auto msg = bst::format("failed to get value of %1%") % key;
    logger.error(msg.str());
    throw std::invalid_argument(msg.str());
  }
  logger.debug("get finish");
}
}  // namespace

template<typename T>
void Config::get_sniffer(const std::string key, T& value) const {
  logger.debug("get_sniffer start");
  // throw std::invalid_argument on failure
  get(key, sniffer_tree_, value);
  logger.debug("get_sniffer finish");
}
template void Config::get_sniffer(const std::string key, std::string& value) const;
template void Config::get_sniffer(const std::string key, unsigned int& value) const;

template<typename T>
void Config::get_notifier(const std::string key, T& value) const {
  logger.debug("get_notifier start");
  // throw std::invalid_argument on failure
  get(key, notifier_tree_, value);
  logger.debug("get_notifier finish");
}
template void Config::get_notifier(const std::string key, std::string& value) const;

Config::Config(const std::string config_file_path) {
  logger.debug("Config start");
  bpt::ptree root_tree;
  // throw bpt::json_parser::json_parser_error on failure
  bpt::read_json(config_file_path, root_tree);
  // throw bpt::ptree_bad_path on failure
  sniffer_tree_ = root_tree.get_child("sniffer");
  notifier_tree_ = root_tree.get_child("notifier");
  logger.debug("Config finish");
}
