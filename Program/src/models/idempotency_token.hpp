#pragma once

#include <string>

#include <models/file_frame.hpp>

namespace models::idempotency_token {

std::string GetToken(const file_frame::FileHeader& file_header,
              const file_frame::FragmentHeader& fragment_header);

}