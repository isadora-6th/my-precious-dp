#pragma once

#include <fstream>
#include <functional>
#include <models/file_frame.hpp>
#include <modules/fs_watcher.hpp>

namespace helpers::file_scheduler {

class FileReader {
 public:
  using ReadCallback = std::function<void(models::file_frame::FileFragment&&)>;
  FileReader(const modules::fs_watcher::FileDiscription& discription,
             uint64_t max_frame_size)
      : max_frame_size_(max_frame_size),
        file_(discription_.path, std::ios::binary),
        discription_(discription) {}

  void Read(ReadCallback callback, models::file_frame::Action action);
  bool OpenOk();

  ~FileReader() { file_.close(); }

 private:
  uint64_t max_frame_size_;
  std::ifstream file_;
  modules::fs_watcher::FileDiscription discription_;
};

}  // namespace helpers::file_scheduler
