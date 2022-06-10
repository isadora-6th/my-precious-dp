#include "file_scheduler.hpp"

#include <models/idempotency_token.hpp>
#include <utils/calculate_hash.hpp>

namespace helpers::file_scheduler {

void FileReader::Read(ReadCallback callback,
                      models::file_frame::Action action) {
  if (!file_.is_open()) {
    return;
  }

  uint64_t parts = discription_.size % max_frame_size_ + 1;

  models::file_frame::FileHeader file_header{
      discription_.path.string(), discription_.size, action,
      discription_.last_write_time, utils::calculate_hash::Calculate(file_)};

  for (uint64_t i = 0; i < parts; i++) {
    auto payload_size = std::min((uint64_t)max_frame_size_,
                                 (discription_.size - (max_frame_size_ * i)));

    std::vector<char> payload;
    payload.reserve(payload_size);

    file_.read(&payload[0], payload_size);
    models::file_frame::FragmentHeader fragment_header{
        i, parts, utils::calculate_hash::Calculate(payload),
        max_frame_size_ * i};

    auto idempotency_token =
        models::idempotency_token::GetToken(file_header, fragment_header);

    models::file_frame::FileFragment file_fragment{
        file_header, std::move(fragment_header), std::move(idempotency_token),
        std::move(payload)};

    callback(std::move(file_fragment));
  }
}
bool FileReader::OpenOk() { return file_.is_open(); }

}  // namespace helpers::file_scheduler