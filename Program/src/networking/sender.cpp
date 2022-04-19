#include "sender.hpp"

namespace networking::sender {

size_t SingleSocketSender::Send(){
    return 0;
}

SingleSocketSender::~SingleSocketSender(){
  socket_.close();
}

}