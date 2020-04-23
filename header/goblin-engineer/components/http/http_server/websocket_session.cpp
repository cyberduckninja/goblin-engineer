#include <goblin-engineer/components/http/http_server/websocket_session.hpp>

#include <iostream>

namespace goblin_engineer { namespace components { namespace http_server {

inline void fail(boost::beast::error_code ec, char const *what) {
  std::cerr << what << ": " << ec.message() << "\n";
}

websocket_session::websocket_session(detail::tcp::socket &&socket,helper_write_f_t pipe_)
    : ws_(std::move(socket))
    , pipe_(pipe_) {}

void websocket_session::on_accept(boost::beast::error_code ec) {
  if (ec)
    return fail(ec, "accept");

  do_read();
}

void websocket_session::on_read(boost::beast::error_code ec,std::size_t bytes_transferred) {
  boost::ignore_unused(bytes_transferred);

  if (ec == detail::websocket::error::closed)
    return;

  if (ec) {
    fail(ec, "read");
  }

  ws_.text(ws_.got_text());
  ws_.async_write(
      buffer_.data(),
      boost::beast::bind_front_handler(&websocket_session::on_write,shared_from_this())
  );
}

void websocket_session::on_write(boost::beast::error_code ec,std::size_t bytes_transferred) {

  boost::ignore_unused(bytes_transferred);

  if (ec)
    return fail(ec, "write");

  buffer_.consume(buffer_.size());

  do_read();
}

void websocket_session::do_read() {
  ws_.async_read(
      buffer_
      , boost::beast::bind_front_handler(&websocket_session::on_read, shared_from_this())
  );
}

}}}