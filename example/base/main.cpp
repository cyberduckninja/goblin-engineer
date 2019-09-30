#include <goblin-engineer.hpp>
#include <goblin-engineer/components/network.hpp>

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio.hpp>
#include <chrono>
#include <ctime>
#include <memory>
#include <iostream>

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

struct data_t final {
    explicit data_t(std::uintptr_t id):id_(id){}
    std::uintptr_t id_ ;
    http::request<http::dynamic_body> request_;
    http::response<http::dynamic_body> response_;
};


class http_connection : public std::enable_shared_from_this<http_connection> {
public:
    http_connection(tcp::socket socket,const actor_zeta::actor_address&address)
        : socket_(std::move(socket))
        , data_(reinterpret_cast<std::uintptr_t>(this))
        , address_(address)
        {}

    void start() {
        read_request();
        check_deadline();
    }

    void write(data_t d){
        auto tmp = std::move(d);
        auto self = shared_from_this();

        tmp.response_.set(http::field::content_length, tmp.response_.body().size());

        http::async_write(
                socket_,
                tmp.response_,
                [self](beast::error_code ec, std::size_t) {
                    self->socket_.shutdown(tcp::socket::shutdown_send, ec);
                    self->deadline_.cancel();
                }
        );
    }

private:
    tcp::socket socket_;

    beast::flat_buffer buffer_{8192};

    data_t data_;

    actor_zeta::actor_address address_;

    net::steady_timer deadline_{socket_.get_executor(), std::chrono::seconds(60)};


    void read_request() {
        auto self = shared_from_this();

        http::async_read(
                socket_,
                buffer_,
                data_.request_,
                [self](beast::error_code ec,std::size_t bytes_transferred) {
                    boost::ignore_unused(bytes_transferred);
                    if (!ec) {
                        self->process_request();
                    }
                }
        );
    }

    void process_request() {
        actor_zeta::send(address_,address_,"router",std::move(data_));
    }

    void check_deadline() {
        auto self = shared_from_this();

        deadline_.async_wait(
                [self](beast::error_code ec) {
                    if (!ec) {
                        self->socket_.close(ec);
                    }
                }
        );
    }
};

using connect_storage_t =  std::unordered_map<std::uintptr_t,std::shared_ptr<http_connection>>;

void http_server(
        tcp::acceptor &acceptor,
        tcp::socket &socket,
        connect_storage_t &storage,
        const actor_zeta::actor_address &address
) {
    acceptor.async_accept(
            socket,
            [&](beast::error_code ec) {
                if (!ec) {
                    auto connect = std::make_shared<http_connection>(std::move(socket), address);
                    storage.emplace(reinterpret_cast<std::uintptr_t>(connect.get()), connect);
                    connect->start();
                }

                http_server(acceptor, socket, storage, address);

            }
    );
}


class http_t final : public goblin_engineer::components::network_manager_service {
public:
    http_t(goblin_engineer::dynamic_config &, goblin_engineer::dynamic_environment *env)
    : network_manager_service(env,"http",1)
    , acceptor(loop(),{tcp::v4(),9999})
    , socket(loop())
    {
        add_handler(
                "write",
                [&](actor_zeta::context & /*ctx*/, data_t & data) -> void {
                    write(std::move(data));
                }
        );

        add_handler(
                "router",
                [&](actor_zeta::context & /*ctx*/, data_t & data) -> void {
                    actor_zeta::send(addresses("worker"),address(),"router",std::move(data));
                }
        );

        http_server(acceptor, socket,connect_storage_,address());

    }

    void write(data_t d){
        data_t tmp = std::move(d);

        auto it =  connect_storage_.find(tmp.id_);

        if( it != connect_storage_.end() ) {
            it->second->write(std::move((tmp)));
        }

    }

    ~http_t() override = default;

private:
    connect_storage_t connect_storage_;
    tcp::acceptor acceptor;
    tcp::socket socket;

};


class worker_t : public goblin_engineer::abstract_service {
public:
    explicit worker_t(http_t *manager) : goblin_engineer::abstract_service(manager, "worker") {
        add_handler(
            "replay",
            [&](actor_zeta::context & /*ctx*/, data_t & data) -> void {
                std::cerr<< "!" << std::endl;
            }
        );
    }

    ~worker_t() override = default;
};


int main() {

    goblin_engineer::dynamic_config cfg;
    goblin_engineer::dynamic_environment app(std::move(cfg));

    auto *http1 = app.add_manager_service<http_t>();
    auto log = goblin_engineer::make_service<worker_t>(http1);

    app.initialize();
    app.startup();

    return 0;
}