#pragma once

#include <boost/asio.hpp>
#include <boost/asio/ip/address.hpp>
#include <boost/asio/ip/udp.hpp>

#include <models/udp_buffer.hpp>
#include <vector>

namespace networking::udp_receiver {

namespace ip = boost::asio::ip;

class UdpReceiver {
public:
  
  


  ip::udp ip_version_;
  ip::address target_ip_;
  ip::port_type target_port_;

private:
  models::udp_buffer::BufferQueue buffer_queue;
  boost::asio::io_service io_service_;
  ip::udp::socket socket{io_service_};
  std::vector<char> recieve_buffer_;
  ip::udp::endpoint remote_endpoint_;
};

}  // namespace networking::udp_receiver