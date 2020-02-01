#include <utility>
#include <unordered_map>
#include <memory>
#include <chrono>

#include <goblin-engineer.hpp>
#include <actor-zeta/core.hpp>
#include <iostream>

#include <goblin-engineer/components/http/detail/network.hpp>
#include <goblin-engineer/components/http/http_server/server.hpp>
#include <goblin-engineer/components/http/http_server/http_session.hpp>


namespace goblin_engineer { namespace components { namespace http_server {

    inline void fail(boost::beast::error_code ec, char const* what){
        std::cerr << what << ": " << ec.message() << "\n";
    }


    using clock = std::chrono::steady_clock;

        class server::impl final : public std::enable_shared_from_this<impl> {
        private:
            struct tcp_listener final  {
                template <typename F>
                tcp_listener(
                        detail::net::io_context& ioc,
                        unsigned short port,
                        F&&f
                )
                    : acceptor_(ioc,boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port),detail::net::socket_base::reuse_address(true))
                    , helper_write(std::forward<F>(f))
                {
                    acceptor_.listen(detail::net::socket_base::max_listen_connections);
                }
                detail::tcp::acceptor acceptor_;
                helper_write_f_t helper_write;
                std::unordered_map<std::size_t, std::shared_ptr<http_session>> storage_session;
            };

            using listener_ptr = std::unique_ptr<tcp_listener>;

        public:
            impl() = default;

            ~impl() = default;

            impl &operator=(impl &&) = default;

            impl(impl &&) = default;

            explicit impl(goblin_engineer::dynamic_config &/*configuration*/) {}

            void run() {
                if (!listener->acceptor_.is_open()) {
                    return;
                }

                do_accept();
            }

            void do_accept() {
                // The new connection gets its own strand
                listener->acceptor_.async_accept(
                        detail::net::make_strand(listener->acceptor_.get_executor()),
                        boost::beast::bind_front_handler(
                                &impl::on_accept,
                                shared_from_this()
                        )
                );
            }

            void on_accept(boost::system::error_code ec,detail::tcp::socket socket) {

                if (ec) {
                    fail(ec, "accept");
                } else {
                    auto id_ = static_cast<std::size_t>(std::chrono::duration_cast<std::chrono::microseconds>(clock::now().time_since_epoch()).count());
                    listener->storage_session.emplace(id_, std::make_shared<http_session>(std::move(socket),id_,listener->helper_write));
                    listener->storage_session.at(id_)->run();
                }

                // Accept another connection
                do_accept();
            }

            void write(detail::response_context_type &body) {

                std::cerr << "id = " << body.id() << std::endl;
                auto it = listener->storage_session.find(body.id());
                if (it != listener->storage_session.end()) {
                    it->second->write(std::move(body.response()));
                }

            }

        public:
            template<typename Fun>
            auto add_listener(
                    detail::net::io_context& ioc,
                    unsigned short port,
                    Fun&&f
            ){
                std::cerr << "Port = " << port << std::endl;
                listener = std::make_unique<tcp_listener>(ioc,port,std::forward<Fun>(f));
            }

            auto shutdown() {
                //if (!listener->acceptor_.is_open()) {
                //    return;
                //} else {
                 ///listener->acceptor_.
                //}
            }

            std::function<void(detail::request_type &&, std::size_t)> helper_write;

        private:
            listener_ptr listener;
        };

        server::server(
                goblin_engineer::root_manager *env,
                goblin_engineer::dynamic_config &configuration,
                multiplexer&multiplexer_
        )
            : network_manager_service(env, "http",1)
            , pimpl(std::make_shared<impl>(configuration ))
        {

            auto http_address = self();

            pimpl->helper_write = [=](detail::request_type &&req, std::size_t session_id) {

                detail::query_context context(std::move(req), session_id, http_address);

                actor_zeta::send(
                        http_address,
                        actor_zeta::messaging::make_message(
                                http_address,
                                "dispatcher",
                                std::move(context)
                        )
                );

            };

            add_handler(
                    "close",
                    [](actor_zeta::context &/*ctx*/, detail::response_context_type &/*body*/) -> void {

                    }
            );

            add_handler(
                    "write",
                    [this](actor_zeta::context &/*ctx*/, detail::response_context_type &body) -> void {
                        pimpl->write(body);
                    }
            );

            add_handler(
                    "dispatcher",
                    [this](actor_zeta::context &ctx, detail::response_context_type &/*body*/) -> void {
                        actor_zeta::send(addresses("dispather"),std::move(ctx.message()));
                    }
            );

            pimpl->add_listener(loop(),multiplexer_.port(),pimpl->helper_write);
            pimpl->run();
        }
}}}