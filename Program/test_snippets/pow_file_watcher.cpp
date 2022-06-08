#include <chrono>
#include <string>
#include <thread>

#include <config/type/fs_watcher.hpp>
#include <modules/fs_watcher.hpp>

#include <utils/logging.hpp>
#include <fmt/core.h>

int main() {
  auto watcher_cfg = config::filesystem_watcher::Default();
  auto watcher = modules::fs_watcher::FileWatcher(watcher_cfg, [](const modules::fs_watcher::FileDiscription& discr, modules::fs_watcher::FileAction act){
    const static auto act_to_string = [](modules::fs_watcher::FileAction& act) {
      switch(act){
        case modules::fs_watcher::FileAction::kCreated:
          return "Created";
        case modules::fs_watcher::FileAction::kModified:
          return "Modified";
        case modules::fs_watcher::FileAction::kDeleted:
          return "Deleted";
      }
    };
    LOG_INFO() << fmt::format("Update> Path: {}, Size: {}, Action: {}\n", discr.path.string(), discr.size, act_to_string(act));
  });

  watcher.List([](const modules::fs_watcher::FileDiscription& discr){
    LOG_INFO() << fmt::format("INIT> Path: {}, Size: {}\n", discr.path.string(), discr.size);
  });

  std::this_thread::sleep_for(std::chrono::minutes(5));
  return 0;
}