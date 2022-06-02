#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/bind/bind.hpp>
#include <thread>
#include <iostream>
#include <models/file.hpp>
#include <networking/udp_receiver.hpp>
#include <networking/udp_sender.hpp>
#include <models/udp_frame_queue.hpp>

/*
#define IPADDRESS "127.0.0.1" // "192.168.1.64"
#define UDP_PORT 13253

using boost::asio::ip::udp;
using boost::asio::ip::address;

void Sender(std::string in) {
    boost::asio::io_service io_service;
    udp::socket socket(io_service);
    udp::endpoint remote_endpoint = udp::endpoint(address::from_string(IPADDRESS), UDP_PORT);
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

    void handle_receive(const boost::system::error_code& error, size_t bytes_transferred) {
        if (error) {
            std::cout << "Receive failed: " << error.message() << "\n";
            return;
        }
        std::cout << "Received: '" << std::string(recv_buffer.begin(), recv_buffer.begin()+bytes_transferred) << "' (" << error.message() << ")\n";

        if (--count > 0) {
            std::cout << "Count: " << count << "\n";
            wait();
        }
    }

    void wait() {
        socket.async_receive_from(boost::asio::buffer(recv_buffer, 1024),
            remote_endpoint,
            boost::bind(&Client::handle_receive, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
    }

    void Receiver()
    {
        socket.open(udp::v4());
        socket.bind(udp::endpoint(address::from_string(IPADDRESS), UDP_PORT));

        wait();

        std::cout << "Receiving\n";
        io_service.run();
        std::cout << "Receiver exit\n";
    }
};

int main(int argc, char *argv[])
{
    Client client;

    std::thread r([&] { client.Receiver(); });

    std::string input = argc>1? argv[1] : "hello world";
    std::cout << "Input is '" << input.c_str() << "'\nSending it to Sender Function...\n";

    for (int i = 0; i < 3; ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        Sender(input);
    }

    r.join();
}

*/

int main()
{
  boost::asio::io_context io;

  io.post([](){
    std::cout << "does";
  });

  return 0;
}