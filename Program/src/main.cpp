/* #include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
#include <iostream>
#include <models/file.hpp>
#include <models/udp_frame_queue.hpp>
#include <networking/udp_receiver.hpp>
#include <networking/udp_sender.hpp>
#include <thread>

#define IPADDRESS "127.0.0.1"  // "192.168.1.64" //192.168.100.31
#define UDP_PORT 13253

using boost::asio::ip::address;
using boost::asio::ip::udp;

void Sender(std::string in) {
  boost::asio::io_service io_service;
  udp::socket socket(io_service);
  udp::endpoint remote_endpoint =
      udp::endpoint(address::from_string(IPADDRESS), UDP_PORT);
  socket.open(udp::v4());

  boost::system::error_code err;

  auto sent = socket.send_to(boost::asio::buffer(in), remote_endpoint, 0, err);
  socket.close();
  std::cout << "Sent Payload --- " << sent << "\n";
}

struct Client {
  boost::asio::io_service io_service;
  udp::socket socket{io_service};
  std::vector<char> recv_buffer = std::vector<char>(1024);
  udp::endpoint remote_endpoint;

  int count = 3;

  void handle_receive(const boost::system::error_code& error,
                      size_t bytes_transferred) {
    if (error) {
      std::cout << "Receive failed: " << error.message() << "\n";
      return;
    }
    std::cout << "WOW: " << remote_endpoint.address();
    std::cout << "Received: '"
              << std::string(recv_buffer.begin(),
                             recv_buffer.begin() + bytes_transferred)
              << "' (" << error.message() << ")\n";

    if (--count > 0) {
      std::cout << "Count: " << count << "\n";
      wait();
    }
  }

  void wait() {
    socket.async_receive_from(
        boost::asio::buffer(recv_buffer, 1024), remote_endpoint,
        [this](const boost::system::error_code& error,
                      size_t bytes_transferred){
                        handle_receive(error, bytes_transferred);
                      });
  }

  void Receiver() {
    socket.open(udp::v4());
    remote_endpoint = udp::endpoint(boost::asio::ip::address_v4::any(),
UDP_PORT); socket.bind(remote_endpoint);

    wait();

    std::cout << "Receiving\n";
    io_service.run();
    std::cout << "Receiver exit\n";
  }
};

int main(int argc, char* argv[]) {
  Client client;

  std::thread r([&] { client.Receiver(); });

  std::string input = argc > 1 ? argv[1] : "hello world";
  std::cout << "Input is '" << input.c_str()
            << "'\nSending it to Sender Function...\n";

  for (int i = 0; i < 3; ++i) {
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    Sender(input);
  }

  r.join();
}
 */

#include <chrono>
#include <string>
#include <thread>

#include <config/type/udp_receiver.hpp>
#include <config/type/udp_sender.hpp>
#include <config/type/fs_watcher.hpp>
#include <networking/udp_receiver.hpp>
#include <networking/udp_sender.hpp>
#include <modules/fs_watcher.hpp>
#include <utils/call_each.hpp>

#include <utils/logging.hpp>
#include <fmt/core.h>

int main() {
  auto watcher_cfg = config::filesystem_watcher::Default();
  auto watcher = modules::fs_watcher::FileWatcher(watcher_cfg, [](const modules::fs_watcher::FileDiscription& discr, modules::fs_watcher::FileAction act){
    const static auto act_to_string = [](modules::fs_watcher::FileAction& act) {
      switch(act){
        case modules::fs_watcher::FileAction::kCreated:
          return "Created";
        case modules::fs_watcher::FileAction::kModified:
          return "Modified";
        case modules::fs_watcher::FileAction::kDeleted:
          return "Deleted";
      }
    };
    LOG_INFO() << fmt::format("Update> Path: {}, Size: {}, Action: {}\n", discr.path.string(), discr.size, act_to_string(act));
  });

  watcher.List([](const modules::fs_watcher::FileDiscription& discr){
    LOG_INFO() << fmt::format("INIT> Path: {}, Size: {}\n", discr.path.string(), discr.size);
  });

  std::this_thread::sleep_for(std::chrono::minutes(5));
  return 0;
}