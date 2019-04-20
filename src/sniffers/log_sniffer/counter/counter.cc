#include <string>
#include <vector>

#include <boost/format.hpp>

#include <log4cpp/Category.hh>

#include "counter.hpp"

namespace {
namespace bst = boost;

log4cpp::Category& logger = log4cpp::Category::getInstance(__FILE__);

int calc_distance(std::vector<time_t> count) {
  logger.debug("calc_distance start");
  int count_size = count.size();

  logger.debug(
    (bst::format("counter size = %1%") % count_size).str());
  if (count_size <= 1) {
    logger.debug("counter size is smaller than 2. return.");
    return 0;
  } else {
    int distance = (int) difftime(count[count.size() - 1], *count.begin());
    logger.debug((bst::format("distance is %1%") % distance).str());
    return distance;
  }
}

void remove(std::vector<time_t> &count) {
  logger.debug("remove start");
  count.erase(count.begin());
  logger.debug("remove finish");
}

int truncate(int interval, std::vector<time_t> count) {
  logger.debug("truncate start");
  int truncate_num = 0;
  while(true) {

    // if count.size is smaller than 2,
    // we can not truncate.
    if (count.size() <= 1) {
      logger.debug("counter size is smaller than 2. return.");
      return truncate_num;
    }

    // calc distance from first to last element
    int distance = calc_distance(count);
    logger.debug((bst::format("distance is %1%") % distance).str());
    if (distance > interval) {
      logger.debug(
        (bst::format("distance is bigger than interval: (%1% > %2%)")
          % distance % interval).str());

      // if distance is longer than interval,
      // remove first element.
      remove(count);
    } else {
      logger.debug(
        (bst::format("distance is smaller than interval: (%1% <= %2%). truncate complete.")
          % distance % interval).str());

      return truncate_num;
    }
  }
}
}   // namespace

Counter::Counter(int threshold, int interval)
  : threshold_(threshold),
    interval_(interval) {}

void Counter::add() {
  logger.debug("add start");
  count_.push_back(time(0));
  logger.debug("add finish");
}

bool Counter::check_exceed() {
  logger.debug("check_exceed start");
  int truncate_num = truncate(interval_, count_);
  bool ret = (count_.size() >= threshold_);
  logger.debug("check_exceed finish");
  return ret;
}
