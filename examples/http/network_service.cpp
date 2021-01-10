#include "network_service.hpp"

auto policy_empty_storage::join(goblin_engineer::actor tmp) -> goblin_engineer::actor_address {
    auto actor = std::move(tmp);
    auto address = actor->address();
    goblin_engineer::link(*this, address);
    return address;
}

auto policy_empty_storage::join(goblin_engineer::intrusive_ptr<goblin_engineer::supervisor> tmp) -> goblin_engineer::actor_address {
    auto supervisor = std::move(tmp);
    auto address = supervisor->address();
    goblin_engineer::link(*this, address);
    return address;
}

auto policy_empty_storage::executor() noexcept -> goblin_engineer::abstract_executor & {
    return *coordinator_;
}

policy_empty_storage::policy_empty_storage(goblin_engineer::string_view view, goblin_engineer::abstract_executor *executor)
        :goblin_engineer::supervisor(view),coordinator_(executor){}

constexpr bool reuse_address = true;

network_service::network_service(goblin_engineer::abstract_executor* executor, net::io_context &ioc, tcp::endpoint endpoint)
        :  manager_empty_storage("network_manager",executor)
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
