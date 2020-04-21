#pragma once

#include <memory>

#include <goblin-engineer/components/http/detail/network.hpp>
#include <boost/beast/core/error.hpp>
#include <boost/asio/ip/tcp.hpp>
#include "forward.hpp"

namespace goblin_engineer { namespace components { namespace http_server {

        class http_session final : public std::enable_shared_from_this<http_session> {
            // This queue is used for HTTP pipelining.
            class queue final {
                enum {
                    // Maximum number of responses we will queue
                            limit = 8
                };

                // The type-erased, saved work item
                struct work {
                    virtual ~work() = default;

                    virtual void operator()() = 0;
                };

                http_session &self_;
                std::vector<std::unique_ptr<work>> items_;

            public:
                explicit queue(http_session &self);

                // Returns `true` if we have reached the queue limit
                bool is_full() const {
                    return items_.size() >= limit;
                }

                // Called when a message finishes sending
                // Returns `true` if the caller should initiate a read
                bool on_write();

                // Called by the HTTP handler to send a response.
                template<bool isRequest, class Body, class Fields>
                void operator()(detail::http::message<isRequest, Body, Fields> &&msg) {
                    // This holds a work item
                    struct work_impl final : work {
                        http_session &self_;
                        detail::http::message<isRequest, Body, Fields> msg_;

                        work_impl(
                                http_session &self,
                                detail::http::message<isRequest, Body, Fields> &&msg)
                                : self_(self)
                                , msg_(std::move(msg))
                        {
                        }

                        void operator()() {
                            detail::http::async_write(
                                    self_.stream_,
                                    msg_,
                                    boost::beast::bind_front_handler(
                                            &http_session::on_write,
                                            self_.shared_from_this(),
                                            msg_.need_eof()
                                    )
                            );
                        }
                    };

                    // Allocate and store the work
                    items_.push_back(std::make_unique<work_impl>(self_, std::move(msg)));

                    // If there was no previous work, start this one
                    if (items_.size() == 1) {
                        (*items_.front())();
                    }
                }
            };

            boost::beast::tcp_stream stream_;
            boost::beast::flat_buffer buffer_;
            queue queue_;
            boost::optional<detail::http::request_parser<detail::http::string_body>> parser_;
            helper_write_f_t handle_processing;
            const std::size_t id;
        public:
            http_session(detail::tcp::socket&&, helper_write_f_t);

            ~http_session() = default;

            void run();

            void do_read();

            void on_read(boost::beast::error_code, std::size_t );

            void on_write(bool close, boost::beast::error_code ec, std::size_t bytes_transferred);

            void do_close();

            void write(detail::response_type &&);

        };

}}}