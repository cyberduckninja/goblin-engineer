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

#include "context.hpp"

#include <goblin-engineer.hpp>
#include <goblin-engineer/http.hpp>


namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace websocket = beast::websocket; // from <boost/beast/websocket.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>



inline void fail(beast::error_code ec, char const *what) {
    std::cerr << what << ": " << ec.message() << "\n";
}

auto thread_pool_deleter = [](actor_zeta::abstract_executor* ptr){
    ptr->stop();
    delete ptr;
};

class network_service final : public goblin_engineer::abstract_manager_service {
public:
    network_service(
            net::io_context &ioc,
            tcp::endpoint endpoint
    );

    ~network_service();

    void run();

    void write(session_id id, goblin_engineer::http::response_type& response);

    void enqueue(goblin_engineer::message msg, goblin_engineer::execution_device *) override;

    auto executor() noexcept -> goblin_engineer::abstract_executor& {
        return *coordinator_;
    }

private:

    auto  get_executor() noexcept -> goblin_engineer::abstract_executor* override {
        return  coordinator_.get();
    }

    std::unique_ptr<actor_zeta::abstract_executor,decltype(thread_pool_deleter)>coordinator_;
    net::io_context &io_context_;
    tcp::acceptor acceptor_;
    std::unique_ptr<network_context> context_;

    void do_accept();

    void on_accept(beast::error_code ec, tcp::socket socket);
};