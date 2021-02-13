#include "network_service.hpp"
#include "http_connection.hpp"

constexpr bool reuse_address = true;

network_service::network_service( net::io_context &ioc, tcp::endpoint endpoint)
        :  goblin_engineer::abstract_manager_service("network_manager")
        , coordinator_(
                new actor_zeta::executor_t<actor_zeta::work_sharing>(
                        1,
                        std::numeric_limits<std::size_t>::max()
                ),
                detail::deleter()
                )
        ,  io_context_(ioc)
        , acceptor_(ioc,endpoint,reuse_address)
        , context_(std::make_unique<network_context>( [this](const goblin_engineer::string_view& name) -> goblin_engineer::actor_address {
        if("self" == name){
            return self();
        }
        return addresses(name);

    })){
    add_handler("write",&network_service::write);
    run();
}

network_service::~network_service() {
    acceptor_.close();
}

void network_service::run() {
    do_accept();
}

void network_service::write(session_id id, goblin_engineer::http::response_type& response) {
    context_->session(id).write(std::move(response));
}


void network_service::do_accept() {
    acceptor_.async_accept(
            net::make_strand(io_context_),
            beast::bind_front_handler(
                    &network_service::on_accept,
                    this));
}

void network_service::on_accept(beast::error_code ec, tcp::socket socket) {
    if (ec) {
        fail(ec, "accept");
    } else {
        auto& session =  context_->session(context_.get(),std::move(socket));
        session.run();
    }

    do_accept();
}

void network_service::enqueue(goblin_engineer::message msg, goblin_engineer::execution_device *) {
    boost::asio::post(
            io_context_,
            [this, msg = std::move(msg)]() mutable {
                set_current_message(std::move(msg));
                dispatch().execute(*this);
            }
    );
}
