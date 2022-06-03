#pragma once

#include <unordered_map>

namespace config::storage {

class Storage {
private:
  Storage();
public:
  Storage& GetInstance();
};

}