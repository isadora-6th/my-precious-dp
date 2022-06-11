#include "udp_sender.hpp"

#include <fmt/core.h>

#include <utils/logging.hpp>

namespace networking::udp_sender {

void UdpSender::Start() {
  sender_thread_ =
      std::make_unique<std::thread>(std::thread([this] { io_service_.run(); }));
}

void UdpSender::Send(std::unique_ptr<UdpSender::Package> data_to_send) {
  if (!data_to_send) {
    throw DataIncorrectState("Data package handles nothing");
  }

  socket_.async_send_to(
      boost::asio::buffer(data_to_send->data), receiver_endpoint_,
      [data_buffer = std::move(data_to_send), this](
          const boost::system::error_code& error,
          size_t bytes_transferred) mutable {
        if (!error && data_buffer->data.size() == bytes_transferred) {
          /*Send succeed*/
          return;
        }

        LOG_WARNING() << fmt::format(
            "Send failed: {} \ndata.size()::{} / actually_sent::{}\n",
            error.what(), data_buffer->data.size(), bytes_transferred);

        if (data_buffer->retries > retry_count_) {
          LOG_WARNING() << "Data sent failed after retries: "
                        << data_buffer->retries;
          return;
        }

        data_buffer->retries += 1;
        Send(std::move(data_buffer));
      });
}

}  // namespace networking::udp_sender
