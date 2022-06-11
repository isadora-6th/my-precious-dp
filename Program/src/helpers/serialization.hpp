#pragma once

#include <helpers/to.hpp>
#include <string>
#include <tuple>
#include <vector>

namespace helpers::serialization {

static const int kSizeBytes = 8;
using SerializedContainer = std::vector<char>;

SerializedContainer Serialize(const int64_t& to_serialize);
int64_t Deserialize(SerializedContainer::const_iterator& start_position, to::To<int64_t>);

SerializedContainer Serialize(const uint64_t& to_serialize);
uint64_t Deserialize(SerializedContainer::const_iterator& start_position,
                     to::To<uint64_t>);

SerializedContainer Serialize(const std::string& to_serialize);
std::string Deserialize(SerializedContainer::const_iterator& start_position,
                        to::To<std::string>);

SerializedContainer Serialize(const std::vector<char>& to_serialize);
std::vector<char> Deserialize(
    SerializedContainer::const_iterator& start_position,
    to::To<std::vector<char>>);

}  // namespace helpers::serialization

