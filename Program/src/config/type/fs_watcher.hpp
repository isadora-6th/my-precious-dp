#pragma once


#include <chrono>
#include <string>
#include <config/storage.hpp>

namespace config::filesystem_watcher {

struct Configuration {
  std::chrono::seconds check_folder_each;
  std::string folder_path;
};

Configuration Get(const ::config::storage::Storage& storage);

Configuration Default();

}