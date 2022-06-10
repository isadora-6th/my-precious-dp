#include "calculate_hash.hpp"

#include <utils/sha256.h>

namespace utils::calculate_hash {

std::string Calculate(std::ifstream& /* file */) {
  return "";
}

std::string Calculate(const std::vector<char>& /* data */) {
  return "";
}

}  // namespace utils::calculate_hash
