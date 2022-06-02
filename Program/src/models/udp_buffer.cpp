#include "udp_buffer.hpp"

#include <fmt/core.h>

namespace models::udp_buffer {


BufferQueue::BufferQueue(int queue_size, int buffer_size) {
  buffer_ring_.reserve(queue_size);

  for (int i = 0; i < queue_size; i++) {
    buffer_ring_.emplace_back(
      std::make_unique<DataBuffer>(false, 0, std::vector<char>(buffer_size))
    );
  }

  current_position_ = buffer_ring_.begin();
}

std::unique_ptr<DataBuffer>& BufferQueue::AquireBuffer() {
  static const auto is_buffer_available = [](const std::unique_ptr<DataBuffer>& data_buffer) {
    return !data_buffer->locked;
  };

  std::lock_guard lock(mutex_);

  // find forward
  {
    const auto free_buffer = std::find_if(
        current_position_, std::end(buffer_ring_), is_buffer_available);

    if (free_buffer != std::end(buffer_ring_)) {
      (*free_buffer)->locked = true;
      current_position_ = free_buffer + 1;
      return *free_buffer;
    }
  }

  // find from begin
  {
    const auto free_buffer = std::find_if(
        std::begin(buffer_ring_), current_position_, is_buffer_available);

    if (free_buffer != current_position_) {
      (*free_buffer)->locked = true;
      current_position_ = free_buffer + 1;
      return *free_buffer;
    }
  }

  throw AllBuffersLocked(
      fmt::format("Can't get free buffer from BufferQueue (size : {})",
                  buffer_ring_.size()));
}

}  // namespace models::udp_buffer