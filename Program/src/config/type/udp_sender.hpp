#pragma once

#include <boost/asio/ip/address.hpp>
#include <boost/asio/ip/udp.hpp>

#include <config/storage.hpp>

namespace config::udp_sender {

namespace ip = boost::asio::ip;

struct Configuration {
  ip::udp ip_version;
  ip::address target_ip;
  ip::port_type target_port;
  int retry_count;
};

Configuration Get(const ::config::storage::Storage& storage);

Configuration Default();

}