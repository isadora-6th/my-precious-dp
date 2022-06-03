#pragma once

#include <boost/asio.hpp>
#include <boost/asio/ip/address.hpp>
#include <boost/asio/ip/udp.hpp>
#include <config/type/udp_receiver.hpp>
#include <functional>
#include <models/udp_buffer.hpp>
#include <vector>

namespace networking::udp_receiver {

namespace ip = boost::asio::ip;



class UdpReceiver {
 public:
  using HandlerType = std::function<void(models::udp_buffer::DataBuffer)>;

  UdpReceiver(const config::udp_receiver::Configuration& config,
              const HandlerType handler)
      : started_(false),
        thread_count_(config.receive_threads),
        ip_version_(config.ip_version),
        buffer_queue_(config.buffer_count, config.datagram_max_size),
        socket_(io_service_),
        remote_endpoint_(config.target_ip, config.target_port),
        handler_(handler){};

  void Start();

  class BufferCorrupted : std::runtime_error {
    using std::runtime_error::runtime_error;
  };

  class BufferSizeLessThanRequired : std::runtime_error {
    using std::runtime_error::runtime_error;
  };

 private:
  void WaitReceive();
  void Handle(std::unique_ptr<models::udp_buffer::DataBuffer> data_buffer,
              const boost::system::error_code& error, size_t bytes_transferred);
  bool started_;
  int thread_count_;
  ip::udp ip_version_;
  models::udp_buffer::BufferQueue buffer_queue_;
  boost::asio::io_service io_service_;
  ip::udp::socket socket_{io_service_};
  ip::udp::endpoint remote_endpoint_;
  std::vector<std::thread> receive_threads_;
  HandlerType handler_;
};

}  // namespace networking::udp_receiver