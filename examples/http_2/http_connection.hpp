#include <iostream>
#include <fstream>
#include <memory>
#include <unordered_map>
#include <functional>

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio.hpp>

#include <boost/stacktrace.hpp>

#include "dto.hpp"
#include "context.hpp"

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

using request_type = http::request<http::dynamic_body>;
using response_type = http::response<http::dynamic_body>;

class http_connection : public std::enable_shared_from_this<http_connection> {
public:
    http_connection(network_context*ctx,tcp::socket socket)
            : context_(ctx)
            , socket_(std::move(socket))
            , id_(reinterpret_cast<std::uintptr_t>(this)) {
    }

    void run() {
        read_request();
    }

    void write(response_type response) {
        auto self = shared_from_this();
        auto response_tmp = std::move(response);
        http::async_write(
                socket_,
                response_tmp,
                [self](beast::error_code ec, std::size_t) {
                    self->socket_.shutdown(tcp::socket::shutdown_send, ec);
                });
    }

private:
    network_context* context_;
    const std::uintptr_t id_;
    tcp::socket socket_;

    beast::flat_buffer buffer_;

    request_type request_;

    void read_request() {
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

    void process_request() {
        http_dto data(id_,std::move(request_));
        context_->dispatch(std::move(data) );

    }
};