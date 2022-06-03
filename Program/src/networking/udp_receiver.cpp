#include "udp_receiver.hpp"

#include <boost/bind/bind.hpp>
#include <utils/Logging.hpp>

namespace networking::udp_receiver {
namespace {
struct RAIIBufferContainer {
  RAIIBufferContainer(
      std::unique_ptr<models::udp_buffer::DataBuffer> buffer_to_return,
      models::udp_buffer::BufferQueue& queue_to_return)
      : container(std::move(buffer_to_return)), queue(queue_to_return) {}
  ~RAIIBufferContainer() { queue.ReleaseBuffer(std::move(container)); }
  std::unique_ptr<models::udp_buffer::DataBuffer> container;
  models::udp_buffer::BufferQueue& queue;
};

}  // namespace

void UdpReceiver::Start() {
  if (started_) {
    LOG_INFO() << "UdpReceiver already started\n";
    return;
  }
  started_ = true;

  socket_.open(ip_version_);
  socket_.bind(receiver_endpoint_);

  for (int i = 0; i < thread_count_; i++) {
    receive_threads_.push_back(std::thread([this] {
      WaitReceive();
      io_service_.run();
    }));
  }
}

void UdpReceiver::WaitReceive() {
  auto buffer = buffer_queue_.AquireBuffer();
  auto endpoint = std::make_unique<ip::udp::endpoint>();

  socket_.async_receive_from(
      boost::asio::buffer(buffer->buffer, buffer->buffer.size()), *endpoint,
      [data_buffer = std::move(buffer), udp_source = std::move(endpoint), this](
          const boost::system::error_code& error,
          size_t bytes_transferred) mutable {
        if (!data_buffer) {
          throw UdpReceiver::BufferCorrupted("Data buffer ptr is NULL");
        }
        if (!udp_source) {
          throw UdpReceiver::BufferCorrupted("UdpSource lost");
        }

        RAIIBufferContainer secured_buffer(std::move(data_buffer),
                                           buffer_queue_);

        Handle(*secured_buffer.container, *udp_source, error,
               bytes_transferred);
      });
}

void UdpReceiver::Handle(models::udp_buffer::DataBuffer& data_buffer,
                         ip::udp::endpoint& udp_source,
                         const boost::system::error_code& error,
                         size_t bytes_transferred) {
  if (error) {
    LOG_WARNING() << "Receive failed: " << error.message() << "\n";
    throw ReceiveFailed(error.message());
  }

  if (expected_source_ip_) {
    if (expected_source_ip_ != udp_source.address()) {
      LOG_INFO() << "Receive blocked for ip: " << udp_source.address() << "\n";
      return;
    }
  }

  if (data_buffer.buffer.size() < bytes_transferred) {
    throw UdpReceiver::BufferSizeLessThanRequired("Data buffer ptr is NULL");
  }
  data_buffer.last_datagram_size = bytes_transferred;

  try {
    handler_(data_buffer);
  } catch (std::runtime_error& e) {
    LOG_INFO() << "Caught untyped exception: " << e.what() << "\n";
  }
}

}  // namespace networking::udp_receiver