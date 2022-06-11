#pragma once

#include <atomic>
#include <exception>
#include <memory>
#include <mutex>
#include <vector>

namespace models::udp_buffer {

struct DataBuffer {
  DataBuffer(int _last_datagram_size, std::vector<char>&& _buffer)
      : last_datagram_size(_last_datagram_size), buffer(_buffer) {}
  int last_datagram_size;
  std::vector<char> buffer;
};

class BufferQueue {
 public:
  class AllBuffersLocked : std::runtime_error {
    using std::runtime_error::runtime_error;
  };

  BufferQueue(int queue_size, int buffer_size);

  std::unique_ptr<DataBuffer> AquireBuffer();
  void ReleaseBuffer(std::unique_ptr<DataBuffer> buffer_to_release);

 private:
  int buffer_size_; 
  std::vector<std::unique_ptr<DataBuffer>> free_buffers_;
  std::mutex mutex_;
};

}  // namespace models::udp_buffer







