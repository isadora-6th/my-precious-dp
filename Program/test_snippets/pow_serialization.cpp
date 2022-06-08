#include <chrono>
#include <string>

#include <helpers/serialization.hpp>
#include <utils/logging.hpp>

int main() {
  const auto serialized = helpers::serialization::Serialize("SErialize Test");
  const auto deserialized = helpers::serialization::Deserialize(serialized.begin(), helpers::serialization::To<std::string>());
  LOG_INFO() << deserialized;
}