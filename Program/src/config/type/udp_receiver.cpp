#include "udp_receiver.hpp"

namespace config::udp_receiver {

Configuration Get(const ::config::storage::Storage& /*storage*/){
  return Default();
}

Configuration Default(){
  return {
    ip::udp::v4(),
    std::nullopt,
    ip::port_type(2022),
    1'048'576 /*datagram_max_size = 1 MiB*/,
    8 /*buffer_count*/,
    4 /*receive_threads*/
  };
}

}