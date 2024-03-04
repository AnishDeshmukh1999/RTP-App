#pragma once
#include <SDKDDKVer.h>

#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/asio/deadline_timer.hpp>
#include <boost/bind.hpp>
#include <iostream>
#include <thread>
#include <utility>

#include "Walnut/Core/Buffer.h"

namespace Networking {
using namespace boost::asio;
using boost::asio::deadline_timer;
using namespace boost::posix_time;
using namespace std::chrono_literals;
using std::this_thread::sleep_for;
class Client {
 public:
  using LogMessageCallback = std::function<void(const std::string&)>;
  using DataReceivedCallback = std::function<void(const std::string&)>;
  Client(std::string address, int port)
      : m_address{address},
        m_port{port},
        m_socket{m_io_context},
        deadline_{m_io_context} {
    // No deadline is required until the first socket operation is started. We
    // set the deadline to positive infinity so that the actor takes no action
    // until a specific deadline is set.
    deadline_.expires_at(boost::posix_time::pos_infin);

    // Start the persistent actor that checks for deadline expiry.
    check_deadline();
  };
  ~Client() {
    if (m_NetworkThread.joinable()) m_NetworkThread.join();
  };
  void check_deadline() {
    // Check whether the deadline has passed. We compare the deadline against
    // the current time since a new asynchronous operation may have moved the
    // deadline before this actor had a chance to run.
    if (deadline_.expires_at() <= deadline_timer::traits_type::now()) {
      // The deadline has passed. The outstanding asynchronous operation needs
      // to be cancelled so that the blocked receive() function will return.
      //
      // Please note that cancel() has portability issues on some versions of
      // Microsoft Windows, and it may be necessary to use close() instead.
      // Consult the documentation for cancel() for further information.
      m_socket.cancel();

      // There is no longer an active deadline. The expiry is set to positive
      // infinity so that the actor takes no action until a new deadline is set.
      deadline_.expires_at(boost::posix_time::pos_infin);
    }

    // Put the actor back to sleep.
    deadline_.async_wait(boost::bind(&Client::check_deadline, this));
  }
  void NetworkThreadFunc();
  void Start();
  void Stop();
  void SetLogMessageCallback(const LogMessageCallback& callback);
  void SetDataReceivedCallback(const DataReceivedCallback& callback);

 private:
  bool m_Running{};
  int m_port{};
  std::thread m_NetworkThread;
  std::string m_address{};
  LogMessageCallback m_LogMessageCallback{nullptr};
  DataReceivedCallback m_DataReceivedCallback{nullptr};
  io_context m_io_context;
  ip::udp::socket m_socket;
  ip::udp::endpoint m_endpoint;
  boost::array<char, 1024> recv_buffer{};
  int count = 5;
  boost::system::error_code m_error;
  deadline_timer deadline_;
  void handle_receive(const boost::system::error_code& ec, std::size_t length,
                      boost::system::error_code* out_ec,
                      std::size_t* out_length);
  void wait(boost::system::error_code& error);
};
}  // namespace Networking
