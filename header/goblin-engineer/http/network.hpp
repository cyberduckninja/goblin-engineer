#pragma once

#include <functional>
#include <sstream>
#include <string>
#include <unordered_map>

#include <boost/beast/http.hpp>

#include "forward.hpp"

namespace goblin_engineer { namespace http {

    using boost::string_view;

    namespace beast = boost::beast;
    namespace http = beast::http;

    class options final {
    public:
        constexpr options() = default;

        ~options() = default;
    };

    class query_context_t final {
    public:
        query_context_t() = default;

        query_context_t(const query_context_t&) = default; // todo hack
        query_context_t& operator=(const query_context_t&) = delete;

        query_context_t(query_context_t&&) = default;

        query_context_t& operator=(query_context_t&&) = default;

        ~query_context_t() = default;

        query_context_t(
            request_t request_,
            size_t i,
            actor_zeta::actor_address address)
            : request_(std::move(request_))
            , id_(i)
            , address(std::move(address)) {
        }

        auto request() -> request_t& {
            return request_;
        }

        auto response() -> response_t& {
            return response_;
        }

        auto write() {
            response_.prepare_payload();
            /*   response_context_t context(std::move(response_), id_);
                actor_zeta::send(
                        address,
                        actor_zeta::make_message(
                                address,
                                "write",
                                std::move(context)

                        )
                );
                */
        }

    private:
        request_t request_;
        session_id id_;
        actor_zeta::actor_address address;
        response_t response_;
    };

}} // namespace goblin_engineer::http