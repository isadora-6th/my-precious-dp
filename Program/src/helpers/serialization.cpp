#include "serialization.hpp"

namespace helpers::serialization {

namespace {

static const int kSizeBytes = 8;

template<typename IntType>
SerializedContainer SerializeInt(const IntType& to_serialize) {
  SerializedContainer out;
  out.reserve(sizeof(IntType));

  for (int offset = sizeof(IntType) * 8 - 8; offset >= 0; offset -= 8) {
    out.push_back(to_serialize & (0xFFULL << (offset)));
  }
  return out;
}

template<typename IntType>
IntType DeserializeInt(const SerializedContainer::const_iterator& start_position) {
  IntType out = 0;

  const auto end = start_position + sizeof(IntType);

  for (auto it = start_position; it != end; it++) {
    out = (out << 8) | *it;
  }

  return out;
}

template<typename Container>
SerializedContainer SerializeContainer(const Container& to_serialize) {
  SerializedContainer container;
  container.reserve(to_serialize.size() + kSizeBytes);
  
  auto size_part = Serialize(to_serialize.size());
  container.insert(container.begin(), size_part.begin(), size_part.end());
  container.insert(container.end(), to_serialize.begin(), to_serialize.end());

  return container;
}

template<typename Container>
Container DeserializeContainer(const SerializedContainer::const_iterator& start_position){
  const auto size = Deserialize(start_position, To<std::uint64_t>());
  const auto string_start = start_position + kSizeBytes;

  Container out(string_start, string_start + size);
  return out;
}
}

SerializedContainer Serialize(const int64_t& to_serialize) {
  return SerializeInt<int64_t>(to_serialize);
}

int64_t Deserialize(const SerializedContainer::const_iterator& start_position,
                    To<int64_t>) {
  return DeserializeInt<int64_t>(start_position);
}

SerializedContainer Serialize(const uint64_t& to_serialize) {
  return SerializeInt<uint64_t>(to_serialize);
}

uint64_t Deserialize(const SerializedContainer::const_iterator& start_position,
                    To<uint64_t>) {
  return DeserializeInt<uint64_t>(start_position);
}

SerializedContainer Serialize(const std::string& to_serialize) {
  return SerializeContainer<std::string>(to_serialize);
}
std::string Deserialize(const SerializedContainer::const_iterator& start_position,
                        To<std::string>){
  return DeserializeContainer<std::string>(start_position);
}

SerializedContainer Serialize(const std::vector<char>& to_serialize) {
  return SerializeContainer<std::vector<char>>(to_serialize);
}
std::vector<char> Deserialize(const SerializedContainer::const_iterator& start_position,
                        To<std::vector<char>>){
  return DeserializeContainer<std::vector<char>>(start_position);
}

}
