#include "listener.hpp"
#include "http_session.hpp"

#include <boost/beast/core/bind_handler.hpp>
#include <iostream>

namespace goblin_engineer { namespace components { namespace http_server {


inline void fail(boost::beast::error_code ec, char const* what){
  std::cerr << what << ": " << ec.message() << "\n";
}

listener::listener(detail::net::io_context &ioc, port_t port,helper_write_f_t fun)
    : ioc_(ioc)
    , acceptor_(
      detail::net::make_strand(ioc)
      ,boost::asio::ip::tcp::endpoint(
          boost::asio::ip::tcp::v4(), port),
          detail::net::socket_base::reuse_address(true)
    )
    , helper_write_(std::move(fun)) {

  detail::beast::error_code ec;

  // Start listening for connections
  acceptor_.listen(detail::net::socket_base::max_listen_connections);
}
void listener::run() {
  // We need to be executing within a strand to perform async operations
  // on the I/O objects in this session. Although not strictly necessary
  // for single-threaded contexts, this example code is written to be
  // thread-safe by default.
  detail::net::dispatch(
      acceptor_.get_executor(),
      detail::beast::bind_front_handler(&listener::do_accept,
                                          this->shared_from_this()));
}
void listener::do_accept() {
  // The new connection gets its own strand
  acceptor_.async_accept(
      detail::net::make_strand(ioc_),
      detail::beast::bind_front_handler(&listener::on_accept, shared_from_this()));
}

void listener::on_accept(detail::beast::error_code ec, detail::tcp::socket socket) {
  if (ec) {
    fail(ec, "accept");
  } else {
    auto connect = std::make_shared<http_session>(std::move(socket),helper_write_);
    session_storage_.emplace(reinterpret_cast<std::uintptr_t>(connect.get()), connect);
    connect->run();
  }

  // Accept another connection
  do_accept();
}

void listener::write(detail::response_context_type &body) {

  std::cerr << "id = " << body.id() << std::endl;
  auto it = session_storage_.find(body.id());
  if (it != session_storage_.end()) {
    it->second->write(std::move(body.response()));
  }

}

}}}