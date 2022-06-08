#pragma once

#include <boost/asio.hpp>
#include <config/type/fs_watcher.hpp>
#include <filesystem>
#include <functional>
#include <optional>
#include <thread>
#include <unordered_map>
#include <utils/call_each.hpp>

namespace modules::fs_watcher {

enum class FileAction { kCreated, kModified, kDeleted };

struct FileDiscription {
  std::filesystem::path path;
  uintmax_t size;
  std::filesystem::file_time_type last_write_time;

  bool operator==(const FileDiscription& other) const {
    return path == other.path && size == other.size &&
           last_write_time == other.last_write_time;
  }
};

class FileWatcher {
 public:
  using FileUpdateHandler =
      std::function<void(const FileDiscription&, FileAction)>;

  FileWatcher(config::filesystem_watcher::Configuration& config,
              std::optional<FileUpdateHandler> handler)
      : folder_path_(config.folder_path), handler_(handler) {
    namespace fs = std::filesystem;

    for (auto& file : fs::recursive_directory_iterator(folder_path_)) {
      if (fs::is_regular_file(file)) {
        cache_[file.path().string()] =
            std::make_unique<FileDiscription>(FileDiscription{
                file.path(), file.file_size(), file.last_write_time()});
      }
    }

    if (handler) {
      caller_ = std::make_unique<utils::call_each::CallEach>(
          [this]() { UpdateFolder(); }, config.check_folder_each);
    }
  }

  void UpdateFolder();

  void List(std::function<void(const FileDiscription&)> info_cb);

 private:
  using FileWatcherCache =
      std::unordered_map<std::string, std::unique_ptr<FileDiscription>>;
  std::string folder_path_;
  std::optional<FileUpdateHandler> handler_;
  std::unique_ptr<utils::call_each::CallEach> caller_;
  std::mutex mutex_;
  FileWatcherCache cache_;
};

}  // namespace modules::filesystem_watcher
// https://solarianprogrammer.com/2019/01/13/cpp-17-filesystem-write-file-watcher-monitor/