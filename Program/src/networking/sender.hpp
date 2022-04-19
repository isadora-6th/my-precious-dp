#pragma once

#include <boost/asio.hpp>
#include <string>
#include <exception>

namespace networking::sender {

using udp = boost::asio::ip::udp;
using address = boost::asio::ip::address;

class SendFailed : public std::runtime_error {
  using std::runtime_error::runtime_error;
};

class SingleSocketSender {
  SingleSocketSender(boost::asio::io_context& context,
                     const udp::endpoint& remote_endpoint,
                     const udp& ip_version)
      : socket_(context), endpoint_(remote_endpoint) {
    socket_.open(ip_version);
  };

  size_t Send();

  ~SingleSocketSender();

 private:
  udp::socket socket_;
  udp::endpoint endpoint_;
};

}  // namespace networking::sender