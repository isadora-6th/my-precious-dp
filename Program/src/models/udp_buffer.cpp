#include "udp_buffer.hpp"

#include <utils/Logging.hpp>

namespace models::udp_buffer {


BufferQueue::BufferQueue(int queue_size, int buffer_size) {
  buffer_size_ = buffer_size;
  
  free_buffers_.reserve(queue_size);

  for (int i = 0; i < queue_size; i++) {
    free_buffers_.emplace_back(
      std::make_unique<DataBuffer>(0, std::vector<char>(buffer_size_))
    );
  }
}

std::unique_ptr<DataBuffer> BufferQueue::AquireBuffer() {
  std::lock_guard lock(mutex_);

  if(free_buffers_.size() == 0){
    buffer_size_+=1;
    LOG_INFO() << "Added extra buffer, now total: " << buffer_size_ << '\n';

    free_buffers_.emplace_back(
      std::make_unique<DataBuffer>(0, std::vector<char>(buffer_size_))
    );
  }

  auto buffer = std::move(free_buffers_.back());
  free_buffers_.pop_back();

  LOG_DEBUG() << "Buffer aquired\n";

  return buffer;
}

void BufferQueue::ReleaseBuffer(std::unique_ptr<DataBuffer> buffer_to_release){
  std::lock_guard lock(mutex_);

  free_buffers_.push_back(std::move(buffer_to_release));
}

}  // namespace models::udp_buffer