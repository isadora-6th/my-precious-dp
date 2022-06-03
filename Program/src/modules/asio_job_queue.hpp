#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
#include <functional>
#include <iostream>
#include <thread>

namespace modules::asio_job_queue {

class AsioJobQueue {
 public:
  AsioJobQueue(std::size_t pool_size) : work_(io_service_) {
    for (std::size_t i = 0; i < pool_size; ++i) {
      threads_.push_back(std::thread([this] { io_service_.run(); }));
    }
  }
  ~AsioJobQueue();

  template <typename Task>
  void Shedule(Task task) {
    const std::lock_guard<std::mutex> lock(mutex_);

    io_service_.post([this, &task]() { Wrapper(std::function<void()>(task)); });
  }

 private:
  void Wrapper(std::function<void()> task);

  boost::asio::io_service io_service_;
  boost::asio::io_service::work work_;
  std::vector<std::thread> threads_;
  std::mutex mutex_;
};

}  // namespace modules::asio_job_queue