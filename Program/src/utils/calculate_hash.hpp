#pragma once

#include <string>
#include <vector>
#include <fstream>

namespace utils::calculate_hash{

std::string Calculate(std::ifstream& file);
std::string Calculate(const std::vector<char>& data);
  
} // namespace utils::calculate_hash
