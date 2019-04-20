#ifndef SLACK_CLIENT_HPP_
#define SLACK_CLIENT_HPP_

#include <string>

#include <curl/curl.h>

class SlackClient {
 public:
  SlackClient(std::string endpoint_url, std::string token,
              std::string channel, std::string user_name,
              std::string icon_url);
  void post(std::string msg);
 private:
  std::string create_body(std::string msg);
  CURL* curl_;
  // settings
  std::string endpoint_url_;
  std::string channel_;
  std::string icon_url_;
  std::string token_;
  std::string user_name_;
};

#endif  // SLACK_CLIENT_HPP_
