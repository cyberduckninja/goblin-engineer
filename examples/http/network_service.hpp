#pragma once

#include <algorithm>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <vector>
#include <unordered_map>

#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/dispatch.hpp>
#include <boost/asio/strand.hpp>

#include "http_connection.hpp"
#include "context.hpp"

#include <goblin-engineer.hpp>


namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace websocket = beast::websocket; // from <boost/beast/websocket.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>


class policy_empty_storage : public goblin_engineer::supervisor {
public:
    policy_empty_storage(goblin_engineer::string_view ,goblin_engineer::abstract_executor* );

    ~policy_empty_storage() override = default;

    auto executor() noexcept -> goblin_engineer::abstract_executor & override;

    auto join(goblin_engineer::actor tmp) -> goblin_engineer::actor_address override;

    auto join(goblin_engineer::intrusive_ptr<goblin_engineer::supervisor> tmp) -> goblin_engineer::actor_address;

private:
    std::unique_ptr<goblin_engineer::abstract_executor> coordinator_;
};

using manager_empty_storage =  goblin_engineer::basic_manager_service_t<policy_empty_storage>;

inline void fail(beast::error_code ec, char const *what) {
    std::cerr << what << ": " << ec.message() << "\n";
}

class network_service final : public manager_empty_storage  {
public:
    network_service(
            goblin_engineer::abstract_executor* ,
            net::io_context &ioc,
            tcp::endpoint endpoint
    );

    ~network_service();

    void run();

    void write(session_id id, goblin_engineer::http::response_type& response);

    void enqueue(goblin_engineer::message msg, goblin_engineer::execution_device *) override;

private:

    net::io_context &io_context_;
    tcp::acceptor acceptor_;
    std::unique_ptr<network_context> context_;

    void do_accept();

    void on_accept(beast::error_code ec, tcp::socket socket);
};