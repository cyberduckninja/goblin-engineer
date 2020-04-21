#pragma once

#include <unordered_map>

#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/beast/core/error.hpp>

#include "forward.hpp"
#include "http_session.hpp"

namespace goblin_engineer { namespace components { namespace http_server {

class listener : public std::enable_shared_from_this<listener> {
public:
  listener(detail::net::io_context &ioc, port_t port,helper_write_f_t);

  // Start accepting incoming connections
  void run();

  void write(detail::response_context_type &body);

private:
  void do_accept();

  void on_accept(detail::beast::error_code ec, detail::tcp::socket socket);

  detail::net::io_context &ioc_;
  detail::tcp::acceptor acceptor_;
  helper_write_f_t helper_write_;
  std::unordered_map<std::uintptr_t, std::shared_ptr<http_session>> session_storage_;
};

}}}