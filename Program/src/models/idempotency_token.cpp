#include "idempotency_token.hpp"

#include <fmt/core.h>

namespace models::idempotency_token {

std::string GetToken(const file_frame::FileHeader& file_header,
                     const file_frame::FragmentHeader& fragment_header) {
  return fmt::format("{}-{}-{}-{}-{}", file_header.path,
                     file_header.last_write_time.time_since_epoch().count(),
                     file_header.size, fragment_header.part,
                     fragment_header.parts);
}

}  // namespace models::idempotency_token