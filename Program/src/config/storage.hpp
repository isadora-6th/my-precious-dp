#pragma once

#include <unordered_map>

namespace config::storage {

enum class Type {
  kUdpReceiver,
  kReceiveBuffer
};



class Storage {
private:
  Storage();
public:
  Storage& GetInstance();
};

}