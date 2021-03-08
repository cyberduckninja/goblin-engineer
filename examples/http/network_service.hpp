#pragma once

#include <iostream>
#include <memory>
#include <string>

#include <boost/beast/core.hpp>


#include "context.hpp"

#include <goblin-engineer/core.hpp>

namespace beast = boost::beast;
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>


namespace detail{
    struct deleter {
        void operator()(actor_zeta::abstract_executor* ptr){
            ptr->stop();
            delete ptr;
        }
    };
}


class network_service final : public goblin_engineer::abstract_manager_service {
public:
    network_service(
            net::io_context &,
            tcp::endpoint
    );

    ~network_service();

    void run();

    void write(goblin_engineer::http::session_id id, goblin_engineer::http::response_t& response);

    void enqueue_base(goblin_engineer::message_ptr msg, goblin_engineer::execution_device *) override;

    auto executor() noexcept -> goblin_engineer::abstract_executor* override;

private:

    auto  get_executor() noexcept -> goblin_engineer::abstract_executor* override;

    std::unique_ptr<actor_zeta::abstract_executor,detail::deleter>coordinator_;
    net::io_context &io_context_;
    tcp::acceptor acceptor_;
    std::unique_ptr<network_context> context_;

    void do_accept();
    void on_accept(beast::error_code ec, tcp::socket socket);
};