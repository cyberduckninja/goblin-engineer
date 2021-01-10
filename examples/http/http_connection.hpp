#pragma once
#include <iostream>
#include <fstream>
#include <memory>
#include <unordered_map>
#include <functional>

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio.hpp>

#include "dto.hpp"
#include "context.hpp"

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>



class http_connection : public std::enable_shared_from_this<http_connection> {
public:
    http_connection(network_context*ctx,tcp::socket socket);

    void run();

    void write(goblin_engineer::http::response_type response);

private:
    network_context* context_;
    tcp::socket socket_;
    const std::uintptr_t id_;
    beast::flat_buffer buffer_;

    goblin_engineer::http::request_type request_;

    void read_request();

    void process_request();
};