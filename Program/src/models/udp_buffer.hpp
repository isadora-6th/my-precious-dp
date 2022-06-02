#pragma once

#include <atomic>
#include <exception>
#include <memory>
#include <mutex>
#include <vector>

namespace models::udp_buffer {

struct DataBuffer {
  DataBuffer(bool _locked, int _last_datagram_size, std::vector<char>&& _buffer)
      : locked(_locked),
        last_datagram_size(_last_datagram_size),
        buffer(_buffer) {}
  std::atomic<bool> locked;
  std::atomic<int> last_datagram_size;
  std::vector<char> buffer;
};

class BufferQueue {
 public:
  class AllBuffersLocked : std::runtime_error {
    using std::runtime_error::runtime_error;
  };

  BufferQueue(int queue_size, int buffer_size);

  std::unique_ptr<DataBuffer>& AquireBuffer();

 private:
  std::vector<std::unique_ptr<DataBuffer>> buffer_ring_;
  std::vector<std::unique_ptr<DataBuffer>>::iterator current_position_;
  std::mutex mutex_;
};

}  // namespace models::udp_buffer