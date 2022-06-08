#include <chrono>
#include <string>
#include <thread>

#include <config/type/udp_receiver.hpp>
#include <config/type/udp_sender.hpp>
#include <networking/udp_receiver.hpp>
#include <networking/udp_sender.hpp>

#include <utils/logging.hpp>

int main() {
  auto receiver_cfg = config::udp_receiver::Default();
  auto sender_cfg = config::udp_sender::Default();

  auto receiver = networking::udp_receiver::UdpReceiver(
      receiver_cfg, [](models::udp_buffer::DataBuffer& data) {
        LOG_INFO() << "RECEIVER_CALLBACK: "<< std::string(
                          std::begin(data.buffer),
                          std::begin(data.buffer) + data.last_datagram_size)
                   << "\n";
      });
  auto sender = networking::udp_sender::UdpSender(sender_cfg);

  receiver.Start();
  sender.Start();

  auto payload = std::string("Some data");
  auto data = std::vector<char>(std::begin(payload), std::end(payload));
  sender.Send(std::make_unique<networking::udp_sender::UdpSender::Package>(
      networking::udp_sender::UdpSender::Package{std::move(data), 0}));

  using namespace std::chrono_literals;
  std::this_thread::sleep_for(2000ms);
  return 0;
}