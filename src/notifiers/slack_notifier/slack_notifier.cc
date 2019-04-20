#include <string>

#include <curl/curl.h>

#include <log4cpp/Category.hh>

/*
#include "../../config/config.hpp"
#include "../../interfaces/notifier/notifier.hpp"   // interface of notifier
#include "slack_client/slack_client.hpp"
*/
#include <config.hpp>
#include <notifier.hpp>   // interface of notifier
#include "slack_client/slack_client.hpp"


namespace {
log4cpp::Category& logger = log4cpp::Category::getInstance (__FILE__);
}   // namespace

class SlackNotifier : public Notifier {
 public:
  SlackNotifier(const Config& config);
  void notify(std::string msg);
 protected:
  std::shared_ptr<SlackClient> slackClient_;
};

SlackNotifier::SlackNotifier(const Config& config) {
  logger.debug("SlackNotifier start");
  // get settings
  std::string endpoint_url;
  std::string token;
  std::string channel;
  std::string user_name;
  std::string icon_url;
  config.get_notifier(std::string("settings.endpoint_url"), endpoint_url);
  logger.debug("settings.endpoint_url: " + endpoint_url);
  config.get_notifier(std::string("settings.token"), token);
  logger.debug("settings.token: " + token);
  config.get_notifier(std::string("settings.channel"), channel);
  logger.debug("settings.channel: " + channel);
  config.get_notifier(std::string("settings.user_name"), user_name);
  logger.debug("settings.user_name: " + user_name);
  config.get_notifier(std::string("settings.icon_url"), icon_url);
  logger.debug("settings.icon_url: " + icon_url);

  // create slack client
  slackClient_ = std::make_shared<SlackClient>(
    endpoint_url, token,
    channel, user_name, icon_url);

  logger.debug("SlackNotifier finish");
}

void SlackNotifier::notify(std::string msg) {
  logger.debug("notify start");
  slackClient_->post(msg);
  logger.debug("notify finish");
  return;
}

extern "C" {
  SlackNotifier* create(const Config config) {
    return new SlackNotifier(config);
  }
}
