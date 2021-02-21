#pragma once

#include <sstream>
#include <unordered_map>
#include <string>
#include <functional>

#include <boost/beast/http.hpp>

#include "forward.hpp"

namespace goblin_engineer { namespace http {

        using boost::string_view;

        namespace beast = boost::beast;
        namespace http = beast::http;
        using http_method = http::verb;

        class options final {
        public:
            constexpr options() = default;

            ~options() = default;
        };


        class response_context_t final {
        public:
            response_context_t() = default;

            response_context_t(const response_context_t &) = default; // todo hack
            response_context_t &operator=(const response_context_t &) = delete;

            response_context_t(response_context_t &&) = default;

            response_context_t &operator=(response_context_t &&) = default;

            ~response_context_t() = default;

            response_context_t(
                    response_t response_,
                    std::size_t i
            ) :
                    response_(std::move(response_)),
                    id_(i) {

            }

            response_context_t(
                    std::size_t i
            ) :
                    id_(i) {

            }


            auto response() -> response_t & {
                return response_;
            }

            auto id() {
                return id_;
            }

        private:
            response_t response_;
            std::uintptr_t id_;

        };


        class query_context final {
        public:
            query_context() = default;

            query_context(const query_context &) = default; // todo hack
            query_context &operator=(const query_context &) = delete;

            query_context(query_context &&) = default;

            query_context &operator=(query_context &&) = default;

            ~query_context() = default;

            query_context(
                    request_t request_,
                    size_t i,
                    actor_zeta::actor_address address
            ) :
                    request_(std::move(request_)),
                    id_(i),
                    address(std::move(address)) {

            }

            auto request() -> request_t & {
                return request_;
            }

            auto response() -> response_t & {
                return response_;
            }

            auto id() {
                return id_;
            }

            /*
            auto response(response_type&&response_) {
                this->response_ = std::move(response_);
            }
            */
            auto write() {
                response_.prepare_payload();
                response_context_t context(std::move(response_), id_);
                actor_zeta::send(
                        address,
                        actor_zeta::make_message(
                                address,
                                "write",
                                std::move(context)

                        )
                );
            }

        private:
            request_t request_;
            std::size_t id_;
            actor_zeta::actor_address address;
            response_t response_;
        };

}}