#pragma once

#include <boost/asio.hpp>
#include <functional>

namespace utils::call_each {

class CallEach {
 public:
  using Handler = std::function<void()>;
  CallEach(Handler handler, std::chrono::seconds delay) : handler_(handler),
      check_each_(delay), thread_([this] {
        Update();
        io_service_.run();
      }) {}

  ~CallEach() {
    io_service_.stop();
    try {
      thread_.join();
    } catch (const std::exception&) {
    }
  }

 private:
  void Update() {
    timer_.expires_from_now(check_each_);
    timer_.async_wait([this](const boost::system::error_code& e) {
      if(!e || e.value() != boost::asio::error::operation_aborted){
        handler_();
      }
      Update();
    });
  }
  Handler handler_;
  boost::asio::io_service io_service_;
  boost::asio::io_service::work work_{io_service_};
  boost::asio::steady_timer timer_{io_service_};
  std::chrono::seconds check_each_;
  std::thread thread_;
};
}  // namespace utils::call_each
