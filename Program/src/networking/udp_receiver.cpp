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

  socket_.async_receive_from(
      boost::asio::buffer(buffer->buffer, buffer->buffer.size()),
      remote_endpoint_,
      [data = std::move(buffer), this](const boost::system::error_code& error,
                                 size_t bytes_transferred) mutable {
        Handle(std::move(data), error, bytes_transferred);
      });
}

void UdpReceiver::Handle(std::unique_ptr<models::udp_buffer::DataBuffer> data_buffer,
            const boost::system::error_code& error,
            size_t bytes_transferred) {
  if(!data_buffer){
    throw UdpReceiver::BufferCorrupted("Data buffer ptr is NULL");
  }
  if(data_buffer->buffer.size() < bytes_transferred){
    throw UdpReceiver::BufferSizeLessThanRequired("Data buffer ptr is NULL");
  }
  data_buffer->last_datagram_size = bytes_transferred;

  try {
    handler_(*data_buffer);
  } catch(std::runtime_error& e) {
    LOG_INFO() << "Caught untyped exception: " << e.what() << "\n";
  }
}

}  // namespace networking::udp_receiver