#include "calculate_hash.hpp"

#include <utils/sha256.h>

namespace utils::calculate_hash {

std::string Calculate(std::ifstream& file) {
  return sha256(file);
}

std::string Calculate(const std::vector<char>& data) {
  return sha256(data);
}

}  // namespace utils::calculate_hash
