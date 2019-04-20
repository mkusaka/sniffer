#include <string>

#include <curl/curl.h>

#include <log4cpp/Category.hh>

#include "slack_client.hpp"

namespace {
log4cpp::Category& logger = log4cpp::Category::getInstance (__FILE__);
}   // namespace

SlackClient::SlackClient(std::string endpoint_url, std::string token,
                         std::string channel, std::string user_name,
                         std::string icon_url) :
    endpoint_url_(endpoint_url),
    token_(token),
    channel_(channel),
    user_name_(user_name),
    icon_url_(icon_url) {}

void SlackClient::post(std::string msg) {
  logger.debug("post start");
  curl_ = curl_easy_init();
  if (curl_) {
    // set endpoint url
    curl_easy_setopt(curl_, CURLOPT_URL, endpoint_url_.c_str());
  }

  // set body
  std::string body = create_body(msg);
  curl_easy_setopt(curl_, CURLOPT_POSTFIELDS, body.c_str());

  // send request
  CURLcode res;
  res = curl_easy_perform(curl_);

  curl_easy_cleanup(curl_);
  logger.debug("post finish");
}

std::string SlackClient::create_body(std::string msg) {
  logger.debug("create_body start");
  std::string body;
  body += "token=" + token_;
  body += "&";
  body += "channel=" + channel_;
  body += "&";
  body += "text=" + msg;
  body += "&";
  body += "username=" + user_name_;
  body += "&";
  body += "icon_url=" + icon_url_;

  logger.debug("create_body finish");
  return body;
}
