#include "udp_frame_queue.hpp"

#include <fmt/core.h>

namespace networking::frame_queue {

void FrameQueue::Add(models::udp_buffer::DataBuffer& frame_buffer) {
  // Perform pre_process checks
  // Structural_binding
  pool_.Shedule([&frame_buffer]() {
    std::cout << "FrameQueue" << frame_buffer.last_datagram_size;
    std::cout << fmt::format("Processed Frame with size: {}\n",
                             frame_buffer.last_datagram_size);
  });
}

}  // namespace networking::frame_queue







