#include "file_frame.hpp"

#include <unordered_map>

namespace models::file_frame {

std::string ToString(const Action& action) {
  switch (action) {
    case Action::kCreate:
      return "create";
    case Action::kModify:
      return "modify";
    case Action::kDelete:
      return "delete";
  }
}
Action Convert(const std::string& from, helpers::to::To<Action>) {
  static const std::unordered_map<std::string, Action> convert{
      {"create", Action::kCreate},
      {"modify", Action::kModify},
      {"delete", Action::kDelete}};
  auto find = convert.find(from);
  if (find == convert.end()) {
    throw helpers::to::ConvertError(std::string("Can't convert Action from: ") +
                                    from);
  }
  return find->second;
}

}  // namespace models::file_frame

namespace helpers::serialization {

SerializedContainer Serialize(
    const models::file_frame::FileHeader& to_serialize) {
  SerializedContainer out;

  auto write_time = to_serialize.last_write_time.time_since_epoch().count();

  auto path_part = Serialize(to_serialize.path);
  auto size_part = Serialize(to_serialize.size);
  auto action_part = Serialize(ToString(to_serialize.action));
  auto last_write_part = Serialize(write_time);
  auto sha_part = Serialize(to_serialize.sha256_hash);

  out.reserve(path_part.size() + size_part.size() + action_part.size() +
              last_write_part.size() + action_part.size());

  out.insert(out.end(), path_part.begin(), path_part.end());
  out.insert(out.end(), size_part.begin(), size_part.end());
  out.insert(out.end(), action_part.begin(), action_part.end());
  out.insert(out.end(), last_write_part.begin(), last_write_part.end());
  out.insert(out.end(), sha_part.begin(), sha_part.end());

  return out;      
}

models::file_frame::FileHeader Deserialize(
    SerializedContainer::const_iterator& start_position,
    to::To<models::file_frame::FileHeader>){

  auto path = Deserialize(start_position, to::To<std::string>());
  auto size = Deserialize(start_position, to::To<uintmax_t>());
  auto action_str = Deserialize(start_position, to::To<std::string>());
  auto last_write_epoch = Deserialize(start_position, to::To<int64_t>());
  auto sha256 = Deserialize(start_position, to::To<std::string>());

  std::chrono::time_point<std::chrono::system_clock, std::chrono::milliseconds>
    last_write_tp{std::chrono::milliseconds{last_write_epoch}};

  return models::file_frame::FileHeader{
    path,
    size,
    Convert(action_str, helpers::to::To<models::file_frame::Action>()),
    std::chrono::file_clock::from_sys(last_write_tp),
    sha256
  };
}

SerializedContainer Serialize(
    const models::file_frame::FragmentHeader& to_serialize) {
  SerializedContainer out;

  auto part = Serialize(to_serialize.part);
  auto parts = Serialize(to_serialize.parts);
  auto part_sha256_hash = Serialize(to_serialize.part_sha256_hash);
  auto write_position = Serialize(to_serialize.write_position);

  out.reserve(part.size() + parts.size() + part_sha256_hash.size() +
              write_position.size());

  out.insert(out.end(), part.begin(), part.end());
  out.insert(out.end(), parts.begin(), parts.end());
  out.insert(out.end(), part_sha256_hash.begin(), part_sha256_hash.end());
  out.insert(out.end(), write_position.begin(), write_position.end());

  return out;      
}

models::file_frame::FragmentHeader Deserialize(
    SerializedContainer::const_iterator& start_position,
    to::To<models::file_frame::FragmentHeader>){

  auto part = Deserialize(start_position, to::To<uint64_t>());
  auto parts = Deserialize(start_position, to::To<uint64_t>());
  auto part_sha256_hash = Deserialize(start_position, to::To<std::string>());
  auto write_position = Deserialize(start_position, to::To<int64_t>());

  return models::file_frame::FragmentHeader{
    part,
    parts,
    part_sha256_hash,
    write_position
  };
}

SerializedContainer Serialize(
    const models::file_frame::FileFragment& to_serialize) {
  SerializedContainer out;

  auto file_header = Serialize(to_serialize.file_header);
  auto fragment_header = Serialize(to_serialize.fragment_header);
  auto payload = Serialize(to_serialize.payload);

  out.reserve(file_header.size() + fragment_header.size() + payload.size());

  out.insert(out.end(), file_header.begin(), file_header.end());
  out.insert(out.end(), fragment_header.begin(), fragment_header.end());
  out.insert(out.end(), payload.begin(), payload.end());

  return out;      
}


models::file_frame::FileFragment Deserialize(
    SerializedContainer::const_iterator& start_position,
    to::To<models::file_frame::FileFragment>){
  
  auto file_header = Deserialize(start_position, to::To<models::file_frame::FileHeader>());
  auto fragment_header = Deserialize(start_position, to::To<models::file_frame::FragmentHeader>());
  auto payload = Deserialize(start_position, to::To<models::file_frame::Payload>());

  return models::file_frame::FileFragment{
    file_header,
    fragment_header,
    payload
  };
}

}  // namespace helpers::serialization