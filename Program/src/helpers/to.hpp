#pragma once

#include <stdexcept>

namespace helpers::to {

template <typename T>
struct To {};

class ConvertError : std::runtime_error {
  using std::runtime_error::runtime_error;
};

class SerializationError : std::runtime_error {
  using std::runtime_error::runtime_error;
};

}  // namespace helpers::to