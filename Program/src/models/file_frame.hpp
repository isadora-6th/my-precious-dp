#pragma once

#include <string>

namespace models::file_frame {
struct FileFrame{
  int64_t part_id;
  int64_t total_parts;

  std::string file_name;
  
};
}

