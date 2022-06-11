#pragma once

#include <boost/asio.hpp>
#include <boost/asio/ip/address.hpp>
#include <boost/asio/ip/udp.hpp>
#include <config/type/udp_sender.hpp>
#include <vector>

namespace networking::udp_sender {

namespace ip = boost::asio::ip;

class UdpSender {
 public:
  struct Package {
    std::vector<char> data;
    int retries;
  };

  class DataIncorrectState : std::runtime_error {
    using std::runtime_error::runtime_error;
  };

  UdpSender(const config::udp_sender::Configuration& config)
      : receiver_endpoint_(config.target_ip, config.target_port),
        retry_count_(config.retry_count) {
    socket_.open(config.ip_version);
  }

  void Start();

  void Send(std::unique_ptr<Package> data_to_send);

  ~UdpSender() {
    io_service_.stop();
    socket_.close();

    try {
      sender_thread_->join();
    } catch (const std::exception&) {
    }
  }

 private:
  ip::udp::endpoint receiver_endpoint_;
  boost::asio::io_service io_service_;
  boost::asio::io_service::work work_{io_service_};
  ip::udp::socket socket_{io_service_};
  std::unique_ptr<std::thread> sender_thread_;
  int retry_count_;
};
}  // namespace networking::udp_sender



