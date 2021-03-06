#include "fs_watcher.hpp"

namespace config::filesystem_watcher {

Configuration Get(const ::config::storage::Storage& /*storage*/){
  return Default();
}

Configuration Default(){
  return {
    std::chrono::seconds(3),
    std::string("C:\\WORK"),
    262144 /*max_fragment_size = 256 KiB*/
  };
}

}