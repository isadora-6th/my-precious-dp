#pragma once

#include <filesystem>

#include <string>
#include <vector>

#include <helpers/serialization.hpp>
#include <helpers/to.hpp>

namespace models::file_frame {

using Payload = std::vector<char>;

enum class Action {
  kCreate,
  kModify,
  kDelete
};

std::string ToString(const Action&);
Action Convert(const std::string&, helpers::to::To<Action>);

struct FileHeader {
  std::string path;
  uintmax_t size;
  Action action;
  std::filesystem::file_time_type last_write_time;
  std::string sha256_hash;
};

struct FragmentHeader {
  uint64_t part;
  uint64_t parts;
  std::string part_sha256_hash;
  int64_t write_position;
};

struct FileFragment {
  FileHeader file_header;
  FragmentHeader fragment_header;
  Payload payload;
};

}  // namespace models::file_frame

namespace helpers::serialization {

SerializedContainer Serialize(
    const models::file_frame::FileFragment& to_serialize);

models::file_frame::FileFragment Deserialize(
    SerializedContainer::const_iterator& start_position,
    to::To<models::file_frame::FileFragment>);

}  // namespace helpers::serialization