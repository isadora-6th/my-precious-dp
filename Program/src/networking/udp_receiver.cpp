#include "udp_receiver.hpp"

#include <boost/bind/bind.hpp>
#include <utils/Logging.hpp>

namespace networking::udp_receiver {

void UdpReceiver::Start() {
  if (started_) {
    LOG_INFO() << "UdpReceiver already started\n";
    return;
  }
  started_ = true;

  socket_.open(ip_version_);
  socket_.bind(remote_endpoint_);

  for (int i = 0; i < thread_count_; i++) {
    receive_threads_.push_back(std::thread([this] {
      WaitReceive();
      io_service_.run();
    }));
  }
}

void UdpReceiver::WaitReceive() {
  std::unique_ptr<models::udp_buffer::DataBuffer> buffer =
      buffer_queue_.AquireBuffer();

  std::unique_ptr<ip::udp::endpoint> endpoint =
      std::make_unique<ip::udp::endpoint>();

  socket_.async_receive_from(
      boost::asio::buffer(buffer->buffer, buffer->buffer.size()), *endpoint,
      [data = std::move(buffer), udp_source = std::move(endpoint), this](
          const boost::system::error_code& error,
          size_t bytes_transferred) mutable {
        Handle(std::move(data), std::move(udp_source), error,
               bytes_transferred);
      });
}

void UdpReceiver::Handle(
    std::unique_ptr<models::udp_buffer::DataBuffer> data_buffer,
    std::unique_ptr<ip::udp::endpoint> udp_source,
    const boost::system::error_code& error, size_t bytes_transferred) {
  if (error) {
    LOG_WARNING() << "Receive failed: " << error.message() << "\n";
    throw ReceiveFailed(error.message());
  }

  if(expected_source_ip){
    if(expected_source_ip != udp_source->address()){
      LOG_INFO() << "Receive blocked for ip: " << udp_source->address() << "\n";
      return;
    }
  }

  if (!data_buffer) {
    throw UdpReceiver::BufferCorrupted("Data buffer ptr is NULL");
  }
  if (data_buffer->buffer.size() < bytes_transferred) {
    throw UdpReceiver::BufferSizeLessThanRequired("Data buffer ptr is NULL");
  }
  data_buffer->last_datagram_size = bytes_transferred;

  try {
    handler_(*data_buffer);
  } catch (std::runtime_error& e) {
    LOG_INFO() << "Caught untyped exception: " << e.what() << "\n";
  }
}

}  // namespace networking::udp_receiver