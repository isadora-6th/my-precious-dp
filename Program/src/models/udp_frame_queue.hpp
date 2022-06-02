#pragma once

#include <boost/asio.hpp>
#include <boost/circular_buffer.hpp>
#include <models/udp_buffer.hpp>
#include <modules/asio_thread_pool.hpp>
#include <vector>

namespace networking::frame_queue {

namespace ip = boost::asio::ip;

class FrameQueue {
public:
  FrameQueue(int workers_count) : strand_(io_service_), pool_(workers_count) {}

  void Add(models::udp_buffer::DataBuffer& frame_buffer);

private:
  void Shedule(models::udp_buffer::DataBuffer& frame_buffer);
  boost::asio::io_service io_service_;
  boost::asio::io_service::strand strand_;
  modules::asio_thread_pool::AsioThreadPool pool_;
  std::mutex mutex_;
};

}  // namespace networking::frame_queue