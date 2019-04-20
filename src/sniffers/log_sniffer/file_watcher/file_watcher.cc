#include <stdexcept>
#include <fstream>
#include <string>
#include <sys/inotify.h>
#include <unistd.h>

#include <boost/format.hpp>

#include <log4cpp/Category.hh>

#include "file_watcher.hpp"

namespace {
namespace bst = boost;
log4cpp::Category& logger = log4cpp::Category::getInstance (__FILE__);

struct inotify_event* get_event(int inotify_fd) {
  logger.debug("get_event start");
  // create inotify_event buffer
  int buffer_size = sizeof(struct inotify_event);
  char buffer[buffer_size];

  // read event
  int length = 0;
  if ((length = read(inotify_fd, buffer, buffer_size)) != buffer_size) {
    logger.debug("get invalid data. return NULL");
    return NULL;
  }
  struct inotify_event* event = (struct inotify_event *) &buffer[0];
  logger.debug("get_event finish");
  return event;
}

std::string read_diff(
  std::string file_path, size_t old_size, size_t new_size) {
  logger.debug("read_diff start");
  // create file stream
  FILE* stream = fopen(file_path.c_str(), "r");
  if (stream == NULL) {
    fclose(stream);
    logger.error("can't open file stream. return.");
    return "";
  }

  // read diff
  int diff_len = new_size - old_size;
  fseek(stream, old_size, SEEK_SET);
  char buffer[diff_len + 1];
  fread(buffer, sizeof(char), diff_len, stream);
  buffer[diff_len] = '\0';

  // close file stream and return
  fclose(stream);
  logger.debug("read_diff finish");
  return buffer;
}
}   // namespace


FileWatcher::FileWatcher(std::string target_log_file) :
  target_log_file_(target_log_file) {
  logger.debug("FileWatcher start");

  // get current file size
  current_file_size_ = get_file_size();

  // inotify_init() function initialize a new inotify instance,
  // and return file descriptor of inotify event queue.
  inotify_fd_ = inotify_init();
  if (inotify_fd_ == -1) {
    throw std::runtime_error("failed to inotify_init()");
  }

  // inotify_add_watch() function adds a new watch,
  // or modifies an existing watch,
  // for the file whose location is specified in second parameter.
  int ret =
    inotify_add_watch(inotify_fd_, target_log_file_.c_str(), IN_ALL_EVENTS);
  if (ret == -1) {
    throw std::runtime_error("failed to inotify_add_watch()");
  }
  logger.debug("FileWatcher finish");
}

FileWatcher::~FileWatcher() {
  logger.debug("~FileWatcher start");
  close(inotify_fd_);
  logger.debug("~FileWatcher finish");
}

size_t FileWatcher::get_file_size() {
  logger.debug("get_file_size start");
  std::ifstream file(target_log_file_, std::ifstream::ate);
  size_t file_size = static_cast<size_t>(file.tellg());
  logger.debug((bst::format("file_size = %1%") % file_size).str());
  logger.debug("get_file_size finish");
  return file_size;
}

std::string FileWatcher::get_diff() {
  logger.debug("get_diff start");

  // watch an event
  while (true) {
    bool moved = false;
    bool modified = false;

    struct inotify_event* event = get_event(inotify_fd_);
    if (event == NULL) {
      throw std::runtime_error("failed to read inotify_event");
    }

    switch(event->mask) {
      case IN_DELETE :
        logger.debug("get IN_DELETE");
        moved = true;
        break;
      case IN_DELETE_SELF :
        logger.debug("get IN_DELETE_SELF");
        moved = true;
        break;
      case IN_MOVED_FROM :
        logger.debug("get IN_MOVED_FROM");
        moved = true;
        break;
      case IN_MOVED_TO :
        logger.debug("get IN_MOVED_TO");
        moved = true;
        break;
      case IN_MODIFY :
        logger.debug("get IN_MODIFY");
        modified = true;
        break;
      case IN_OPEN :
        logger.debug("get IN_OPEN");
        continue;
      default :
        logger.debug("get unknown event");
        continue;
    }

    // if file was modified
    if (modified) {
      // read diff
      size_t old_file_size = current_file_size_;
      current_file_size_ = get_file_size();
      std::string diff = 
        read_diff(target_log_file_, old_file_size, current_file_size_);

      logger.debug("get_diff finish");
      return diff;
    }

    if (moved) {
      while (true) {
        int tmp_inotify_fd = inotify_init();
        if (tmp_inotify_fd != -1) {
          inotify_fd_ = tmp_inotify_fd;
          break;
        }
        sleep(1);
      }
    }
  }
}
