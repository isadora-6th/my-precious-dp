#pragma once

#include <boost/asio/ip/address.hpp>
#include <boost/asio/ip/udp.hpp>

#include <config/storage.hpp>

namespace config::udp_receiver {

namespace ip = boost::asio::ip;

struct Configuration {
  ip::udp ip_version; // udp::v4()
  ip::address target_ip;
  ip::port_type target_port;
  int datagram_max_size;
  int buffer_count;
  int receive_threads;
};

Configuration Get(const ::config::storage::Storage& storage);

Configuration Default();

}