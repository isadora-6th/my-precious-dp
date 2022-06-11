#include "fs_watcher.hpp"

namespace modules::fs_watcher {

void FileWatcher::UpdateFolder() {
  namespace fs = std::filesystem;

  const std::lock_guard<std::mutex> lock(mutex_);

  FileWatcherCache prev_cache_state_;
  cache_.swap(prev_cache_state_);

  for (auto& file :
       std::filesystem::recursive_directory_iterator(folder_path_)) {
    if (!fs::is_regular_file(file)) {
      continue;
    }
    auto current_file_last_write_time = std::filesystem::last_write_time(file);
    auto file_size = file.file_size();

    auto prev_file_state = prev_cache_state_.find(file.path().string());

    auto file_discription =
        FileDiscription{file.path(), file_size, current_file_last_write_time};

    if (prev_file_state == prev_cache_state_.end()) {
      // New file
      if (handler_) {
        handler_.value()(file_discription, FileAction::kCreated);
      }
      cache_[file.path().string()] =
          std::make_unique<FileDiscription>(std::move(file_discription));

    } else {
      // Exists
      if (!(*prev_file_state->second == file_discription)) {
        // Has changes
        if (handler_) {
          handler_.value()(file_discription, FileAction::kModified);
        }
      }
      prev_cache_state_.erase(prev_file_state);
    }

    cache_[file.path().string()] =
        std::make_unique<FileDiscription>(std::move(file_discription));
  }

  for(const auto& [path, description] : prev_cache_state_){
    if (handler_) {
      handler_.value()(*description, FileAction::kDeleted);
    }
  }
}

void FileWatcher::List(std::function<void(const FileDiscription&)> info_cb){
  const std::lock_guard<std::mutex> lock(mutex_);

  for(const auto& [path, description] : cache_){
    info_cb(*description);
  }
}

}  // namespace modules::filesystem_watcher





