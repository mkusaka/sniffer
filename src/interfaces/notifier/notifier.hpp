#include <string>

class Notifier {
 public:
  Notifier() {}
  virtual ~Notifier() {}
  virtual void notify(std::string msg) {}
};
