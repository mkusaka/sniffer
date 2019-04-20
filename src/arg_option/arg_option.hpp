#ifndef ARG_OPTION_HPP_
#define ARG_OPTION_HPP_

#include <string>

#include <boost/program_options.hpp>

class ArgOption {
 private:
  boost::program_options::options_description description_;
  boost::program_options::variables_map var_map_;
 public:
  ArgOption(const int argc, const char** argv);
  bool count(const std::string key);
  void get(const std::string key, std::string& value);

  friend std::ostream& operator<<(std::ostream& out, ArgOption& argOption);
};
#endif  // ARG_OPTION_HPP_
