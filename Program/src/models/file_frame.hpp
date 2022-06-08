#pragma once

#include <string>
#include <filesystem>
#include <cryptopp/sha.h>
#include <helpers/serialization.hpp>

namespace models::file_frame {

struct FileHeader{
  std::string path;
  uintmax_t size;
  std::filesystem::file_time_type last_write_time;
  std::string sha256_hash;
};

struct FragmentHeader{
  uint64_t part;
  uint64_t parts;
  std::string part_sha256_hash;
};

}

