#include <memory>
#include <iostream>

#include <goblin-engineer.hpp>

#include "network_service.hpp"

auto thread_pool_deleter = [](actor_zeta::abstract_executor* ptr){
    ptr->stop();
    delete ptr;
};

int main(int /*argc*/, char **/*argv[]*/) {

    auto const address = net::ip::make_address("0.0.0.0");
    auto const port = static_cast<unsigned short>(9999);
    auto const threads = std::max<int>(1, 1);

    net::io_context ioc{threads};
    std::unique_ptr<actor_zeta::abstract_executor,decltype(thread_pool_deleter)> executor(
            new actor_zeta::executor_t<actor_zeta::work_sharing>(
                    1,
                    std::numeric_limits<std::size_t>::max()
            ),
            thread_pool_deleter
    );

    auto nm = goblin_engineer::make_manager_service<network_service>(executor.get(),ioc, tcp::endpoint{address, port});
    ///auto mq = make_manager_service<manager_queue>(executor.get());
    ///nm->join(mq);

    std::vector<std::thread> v;
    v.reserve(threads - 1);
    for (auto i = threads - 1; i > 0; --i) {
        v.emplace_back(
                [&ioc] {
                    ioc.run();
                }
        );
    }

    executor->start();
    ioc.run();

    return EXIT_SUCCESS;
}

/*
using connections_type = std::unordered_map<std::uintptr_t,std::shared_ptr<http_connection>>;

void http_server(connections_type&connections,write_helper_type&& f,tcp::acceptor& acceptor, tcp::socket& socket) {
    acceptor.async_accept(
            socket,
            [&](beast::error_code ec) {
                if (!ec) {
                 //   std::make_shared<http_connection>(std::move(socket))->start();
                    auto connect = std::make_shared<http_connection>(std::move(socket),f);
                    connections.emplace(reinterpret_cast<std::uintptr_t>(connect.get()), connect);
                    connect->run();
                }
                http_server(connections,f,acceptor, socket);
            }
    );
}

void write(connections_type&connections,) {

std::cerr << "id = " << body.id() << std::endl;
auto it = connections.find(body.id());
if (it != connections.end()) {
it->second->write(std::move(body.response()));
}

}


class dispatcher {
    void http_get(boost::string_view url, ){

    }
};



int main(int argc, char* argv[]) {
    try {
        net::io_context ioc{1};

        tcp::acceptor acceptor{ioc, {net::ip::make_address("0.0.0.0"), 9999}};
        tcp::socket socket{ioc};
        connections_type connections;
        auto helper = [&connections]() mutable {
           
        };
        http_server(connections,acceptor, socket);

        auto sigint_set = std::make_shared<boost::asio::signal_set>(ioc, SIGINT, SIGTERM);
        sigint_set->async_wait(
                [sigint_set](const boost::system::error_code& , int ) {
                    sigint_set->cancel();
                });
        ioc.run();

    } catch (std::exception const& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
}
*/