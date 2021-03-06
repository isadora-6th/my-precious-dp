#pragma once

#include <boost/asio.hpp>
#include <boost/asio/ip/address.hpp>
#include <boost/asio/ip/udp.hpp>
#include <config/type/udp_receiver.hpp>
#include <functional>
#include <models/udp_buffer.hpp>
#include <optional>
#include <vector>

namespace networking::udp_receiver {

namespace ip = boost::asio::ip;

class UdpReceiver {
 public:
  using HandlerType = std::function<void(models::udp_buffer::DataBuffer&)>;

  UdpReceiver(const config::udp_receiver::Configuration& config,
              const HandlerType handler)
      : started_(false),
        thread_count_(config.receive_threads),
        ip_version_(config.ip_version),
        buffer_queue_(config.buffer_count, config.datagram_max_size),
        socket_(io_service_),
        receiver_endpoint_(boost::asio::ip::address_v4::any(),
                           config.target_port),
        expected_source_ip_(config.target_ip),
        handler_(handler){};

  void Start();

  class ReceiveFailed : std::runtime_error {
    using std::runtime_error::runtime_error;
  };

  class BufferCorrupted : std::runtime_error {
    using std::runtime_error::runtime_error;
  };

  class BufferSizeLessThanRequired : std::runtime_error {
    using std::runtime_error::runtime_error;
  };

  ~UdpReceiver() {
    io_service_.stop();
    socket_.close();

    for (auto& t : receive_threads_) {
      //End threads and supress exceptions if present;
      try {
        t.join();
      } catch (const std::exception&) {}
    }
  }

 private:
  void WaitReceive();
  void Handle(models::udp_buffer::DataBuffer& data_buffer,
              ip::udp::endpoint& udp_source,
              const boost::system::error_code& error, size_t bytes_transferred);

  bool started_;
  int thread_count_;
  ip::udp ip_version_;
  models::udp_buffer::BufferQueue buffer_queue_;
  boost::asio::io_service io_service_;
  ip::udp::socket socket_{io_service_};
  boost::asio::io_service::work work_{io_service_};
  ip::udp::endpoint receiver_endpoint_;
  std::optional<ip::address> expected_source_ip_;
  std::vector<std::thread> receive_threads_;
  HandlerType handler_;
};

}  // namespace networking::udp_receiver