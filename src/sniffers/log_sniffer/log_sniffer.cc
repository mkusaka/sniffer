#include <memory>
#include <regex>
#include <vector>

#include <log4cpp/Category.hh>

#include "counter/counter.hpp"
#include "file_watcher/file_watcher.hpp"
#include <config.hpp>
#include <notifier.hpp>
#include <sniffer.hpp>

namespace {
log4cpp::Category& logger = log4cpp::Category::getInstance (__FILE__);

bool compare(const std::string actual, const std::string regex_pattern) {
  logger.debug("compare start");

  std::regex pattern(regex_pattern);

  bool match = std::regex_match(actual, pattern);

  logger.debug("compare finish");
  return match;
}

std::vector<std::string> split(const std::string str, const std::string separator) {
  logger.debug("split start");
  logger.debug("original string: \"" + str + "\"");

  std::vector<std::string> split_strs;
  std::string tmp_str = str;
  while(true) {
    std::string::size_type pos = tmp_str.find(separator);
    if (pos == std::string::npos) {
      logger.debug("separator is not found. break");
      split_strs.push_back(tmp_str);
      break;
    }
    std::string split_str = (tmp_str.substr(0, pos));
    logger.debug("split string: \"" + split_str + "\"");
    split_strs.push_back(split_str);
    if (pos >= tmp_str.length() - 1) {
      logger.debug("split position is bigger than string length. break");
      break;
    }
    std::string next_tmp_str = tmp_str.substr(pos + 1);
    logger.debug("next original string: \"" + next_tmp_str + "\"");
    tmp_str = tmp_str.substr(pos + 1);
  }
  logger.debug("split finish");
  return split_strs;
}
}   // namespace

class LogSniffer : public Sniffer {
 public:
  LogSniffer(Config config, std::shared_ptr<Notifier> notifier);
  void sniff();
 private:
  // notifier impl
  std::shared_ptr<Notifier> notifier_;
  // file watcher
  std::shared_ptr<FileWatcher> watcher_;
  // counter
  std::shared_ptr<Counter> counter_;
  // settings
  std::string target_regex_pattern_;
};

LogSniffer::LogSniffer(Config config, std::shared_ptr<Notifier> notifier) :
    notifier_(notifier) {
  logger.debug("LogSniffer start");

  std::string target_log_file;
  unsigned int threshold = 0;
  unsigned int interval_sec = 0;

  config.get_sniffer(
    std::string("settings.target_log_file"),
    target_log_file);
  config.get_sniffer(
    std::string("settings.target_regex_pattern"),
    target_regex_pattern_);
  config.get_sniffer(
    std::string("settings.threshold"),
    threshold);
  config.get_sniffer(
    std::string("settings.interval_sec"),
    interval_sec);

  watcher_ = std::make_shared<FileWatcher>(target_log_file);
  counter_ = std::make_shared<Counter>(threshold, interval_sec);

  logger.debug("LogSniffer finish");
}

void LogSniffer::sniff() {
  logger.debug("sniff start");

  while(true) {
    std::string diff = watcher_->get_diff();
    logger.debug("watcher get new logs: \"" + diff + "\"");
    if (diff == "") {
      logger.debug("difference is 0 length. continue.");
      continue;
    }
    std::vector<std::string> each_line = split(diff, "\n");

    for(const auto& new_log : each_line) {
      logger.debug("new log: \"" + new_log + "\"");

      bool ret = compare(new_log, target_regex_pattern_);
      if (ret) {
        logger.debug("new log matches the pattern");
        counter_->add();
        bool exceed = counter_->check_exceed();

        if (exceed) {
          logger.debug("counter exceed the threshold. create notification");

          notifier_->notify(
            "```\n" +
            new_log +
            "```");
        }
      }
      logger.debug("new log does not matche the pattern");
    }
  }
  logger.debug("sniff finish");
}

extern "C" {
  LogSniffer* create(
      const Config config, const std::shared_ptr<Notifier> notifier) {
    return new LogSniffer(config, notifier);
  }
}
