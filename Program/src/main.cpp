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

#include <helpers/serialization.hpp>
#include <models/file_frame.hpp>
#include <utils/logging.hpp>
#include <helpers/file_scheduler.hpp>
#include <utils/calculate_hash.hpp>

int main() {
  const auto serialized = helpers::serialization::Serialize("SErialize Test");
  auto serialized_start = serialized.begin();
  const auto deserialized = helpers::serialization::Deserialize(serialized_start, helpers::to::To<std::string>());
  LOG_INFO() << deserialized;
}
