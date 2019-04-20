#ifndef FILE_WATCHER_HPP_
#define FILE_WATCHER_HPP_

#include <string>
#include <sys/inotify.h>

class FileWatcher {
 public:
  FileWatcher(std::string target_log_file);
  ~FileWatcher();
  std::string get_diff();
  size_t get_file_size();
 private:
  int inotify_fd_;
  std::string target_log_file_;
  size_t current_file_size_;
};

#endif  // FILE_WATCHER_HPP_
