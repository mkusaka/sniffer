#ifndef COUNTER_HPP_
#define COUNTER_HPP_

#include <string>
#include <vector>

class Counter {
 private:
  int threshold_;
  int interval_;
  std::vector<time_t> count_;
 public:
  Counter(int threshold, int interval);
  void add();
  bool check_exceed();
};
#endif
