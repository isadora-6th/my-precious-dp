#pragma once

#include <vector>
#include <string>

namespace helpers::serialization {

using SerializedContainer = std::vector<char>;

template<typename T>
struct To{};

SerializedContainer Serialize(const int64_t& to_serialize);
int64_t Deserialize(const SerializedContainer::const_iterator& start_position, To<int64_t>);

SerializedContainer Serialize(const uint64_t& to_serialize);
uint64_t Deserialize(const SerializedContainer::const_iterator& start_position, To<uint64_t>);

SerializedContainer Serialize(const std::string& to_serialize);
std::string Deserialize(const SerializedContainer::const_iterator& start_position, To<std::string>);

SerializedContainer Serialize(const std::vector<char>& to_serialize);
std::vector<char> Deserialize(const SerializedContainer::const_iterator& start_position, To<std::vector<char>>);


}
