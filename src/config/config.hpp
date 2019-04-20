#ifndef CONFIG_HPP_
#define CONFIG_HPP_

#include <string>

#include <boost/property_tree/ptree.hpp>

class Config {
 private:
  boost::property_tree::ptree sniffer_tree_;
  boost::property_tree::ptree notifier_tree_;
 public:
  Config (const std::string config_file_path);

  template<typename T>
  void get_sniffer(const std::string key, T& value) const;

  template<typename T>
  void get_notifier(const std::string key, T& value) const;
};

#endif  // CONFIG_HPP_
