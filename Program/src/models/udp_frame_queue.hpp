#pragma once

#include <boost/asio.hpp>
#include <boost/circular_buffer.hpp>
#include <models/udp_buffer.hpp>
#include <modules/asio_job_queue.hpp>
#include <vector>

namespace networking::frame_queue {

namespace ip = boost::asio::ip;

class FrameQueue {
public:
  FrameQueue(int workers_count) : strand_(io_service_), pool_(workers_count) {}

  void Add(models::udp_buffer::DataBuffer& frame_buffer);

private:
  boost::asio::io_service io_service_;
  boost::asio::io_service::strand strand_;
  modules::asio_job_queue::AsioJobQueue pool_;
  std::mutex mutex_;
};

}  // namespace networking::frame_queue