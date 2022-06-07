#include "udp_sender.hpp"

namespace config::udp_sender {

Configuration Get(const ::config::storage::Storage& /*storage*/){
  return Default();
}

Configuration Default(){
  return {
    ip::udp::v4(),
    ip::address::from_string("127.0.0.1"),
    ip::port_type(2022),
    5 /*retry_count*/
  };
}

}