#include "asio_job_queue.hpp"

namespace modules::asio_job_queue {

void AsioJobQueue::Wrapper(std::function<void()> task) {
  try {
    task();
  } catch (const std::exception& e) {
    std::cout << "Exception caught in wrapper" << e.what() << "\n";
  } catch (const std::runtime_error& e) {
    std::cout << "Exception caught in wrapper" << e.what() << "\n";
  }
}

}  // namespace modules::asio_job_queue



