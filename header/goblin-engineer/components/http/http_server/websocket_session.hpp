#pragma once

#include <memory>

#include <goblin-engineer/components/http/detail/network.hpp>
#include <boost/beast/http/message.hpp>
#include "forward.hpp"

namespace goblin_engineer { namespace components { namespace http_server {

        class websocket_session final : public std::enable_shared_from_this<websocket_session> {
                detail::websocket::stream<boost::beast::tcp_stream> ws_;
                boost::beast::flat_buffer buffer_;
                helper_write_f_t pipe_;

            public:
                websocket_session(detail::tcp::socket&& socket, helper_write_f_t pipe_);

                // Start the asynchronous operation
                template<class Body, class Allocator>
                void do_accept(detail::http::request<Body, detail::http::basic_fields<Allocator>> req) {
                    // Set suggested timeout settings for the websocket
                    ws_.set_option(detail::websocket::stream_base::timeout::suggested(boost::beast::role_type::server));

                    // Set a decorator to change the Server of the handshake
                    ////ws_.set_option(network::websocket::stream_base::decorator());

                    // Accept the websocket handshake
                    ws_.async_accept(
                            req,
                            boost::beast::bind_front_handler(
                                    &websocket_session::on_accept,
                                    shared_from_this()));
                }

                void on_accept(boost::system::error_code ec);

                void do_read();

                void on_read(boost::beast::error_code ec, std::size_t bytes_transferred);

                void on_write(boost::beast::error_code ec, std::size_t bytes_transferred);
            };
}}}