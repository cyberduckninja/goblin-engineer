#include "http_connection.hpp"

http_connection::http_connection(network_context *ctx, tcp::socket socket)
        : context_(ctx)
        , socket_(std::move(socket))
        , id_(reinterpret_cast<std::uintptr_t>(this)) {
}

void http_connection::run() {
    read_request();
}

void http_connection::write(goblin_engineer::http::response_t response) {
    auto self = shared_from_this();
    auto response_tmp = std::move(response);
    http::async_write(
            socket_,
            response_tmp,
            [self](beast::error_code ec, std::size_t) {
                self->socket_.shutdown(tcp::socket::shutdown_send, ec);
            });
}

void http_connection::process_request() {
    context_->dispatch( http_dto(id_,std::move(request_)));
}

void http_connection::read_request() {
    auto self = shared_from_this();

    http::async_read(
            socket_,
            buffer_,
            request_,
            [self](beast::error_code ec,std::size_t bytes_transferred) {
                boost::ignore_unused(bytes_transferred);
                if (!ec) {
                    self->process_request();
                }
            });
}
