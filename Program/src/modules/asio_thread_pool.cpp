#include "asio_thread_pool.hpp"

namespace modules::asio_thread_pool {

void AsioThreadPool::Wrapper(std::function<void()> task) {
  try {
    task();
  }
  catch (const std::exception& e) {
    std::cout << "Exception caught in wrapper" << e.what() << "\n";
  }
  catch (const std::runtime_error& e) {
    std::cout << "Exception caught in wrapper" << e.what() << "\n";
  }
}

}

