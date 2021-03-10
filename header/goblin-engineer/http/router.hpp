#pragma once

#include <sstream>
#include <unordered_map>
#include <string>
#include <functional>

#include <goblin-engineer/http/network.hpp>

namespace goblin_engineer { namespace http {

    struct http_method_hasher final {
        std::size_t operator()(const method_t &k) const {
            using std::size_t;
            using std::hash;

            return (hash<uint64_t>()(static_cast<uint64_t>(k)));
        }
    };

    struct url_hasher final {
        std::size_t operator()(const boost::string_view &k) const {
            return boost::hash_value(k);
        }
    };


    class http_method_container final {
    public:

        http_method_container() = default;

        http_method_container(http_method_container &&) = default;

        ~http_method_container() = default;

        using action = std::function<void(query_context_t & )>;
        using storage = std::unordered_map<std::string, action>;
        using iterator = storage::iterator;

        template<class F>
        auto registration_handler(
                boost::string_view route_path,
                const options &/*options*/,
                F handler
        ) {
            storage_.emplace(route_path.to_string(), std::forward<F>(handler));
        }

        auto invoke(query_context_t &r) {
            auto url = r.request().target().to_string();
            auto it = storage_.find(url);
            auto request = std::move(r);
            it->second(request);
        }

        auto update(http_method_container &r) {
            storage_.insert(
                    std::make_move_iterator(r.begin()),
                    std::make_move_iterator(r.end())
            );
        }

        auto begin() -> iterator {
            return storage_.begin();
        }

        auto end() -> iterator {
            return storage_.end();
        }

    private:
        storage storage_;
    };

    class router final {
    public:
        router() = default;

        router(router &&) = default;

        router &operator=(router &&) = default;

        ~router() = default;

        using storage = std::unordered_map<method_t, http_method_container, http_method_hasher>;
        using iterator = storage::iterator;

        template<class F>
        auto registration_handler(
                method_t method,
                boost::string_view route_path,
                const options &options,
                F handler
        ) {

            auto it = storage_.find(method);
            if (it == storage_.end()) {
                auto result = storage_.emplace(method, http_method_container());
                result.first->second.registration_handler(route_path, options, std::forward<F>(handler));
            } else {
                it->second.registration_handler(route_path, options, std::forward<F>(handler));
            }
        }

        auto invoke(query_context_t &context) {
            auto method = context.request().method();

            auto it = storage_.find(method);
            it->second.invoke(context);
        }
        /// TODO: not implement update
/*
        auto update(dispatcher&r){
           for(auto&&i:r ){

           }
        }
*/
        auto begin() -> iterator {
            return storage_.begin();
        }

        auto end() -> iterator {
            return storage_.end();
        }

    private:
        storage storage_;
    };

    class wrapper_router final {
    public:
        wrapper_router() = default;

        wrapper_router(wrapper_router &&) = default;

        ~wrapper_router() = default;

        auto get_router() -> router {
            return std::move(router_);
        }

        template<typename F>
        auto http_delete(
                boost::string_view route_path,
                F handler
        ) {
            add_handler(
                    method_t::delete_,
                    route_path,
                    std::move(handler)
            );
        }

        template<typename F>
        auto http_delete(
                boost::string_view route_path,
                const options &options,
                F handler
        ) {
            add_handler(
                    method_t::delete_,
                    route_path,
                    options,
                    std::move(handler)
            );
        }

        template<typename F>
        auto http_get(
                boost::string_view route_path,
                F handler
        ) {
            add_handler(
                    method_t::get,
                    route_path,
                    std::forward<F>(handler)
            );
        }

        template<typename F>
        auto http_get(
                boost::string_view route_path,
                const options &options,
                F handler
        ) {
            add_handler(
                    method_t::get,
                    route_path,
                    options,
                    std::move(handler)
            );
        }

        template<typename F>
        auto http_head(
                boost::string_view route_path,
                F handler
        ) {
            add_handler(
                    method_t::head,
                    route_path,
                    std::move(handler)
            );
        }

        template<typename F>
        auto http_head(
                boost::string_view route_path,
                const options &options,
                F handler
        ) {
            add_handler(
                    method_t::head,
                    route_path,
                    options,
                    std::move(handler)
            );
        }

        template<typename F>
        auto http_post(
                boost::string_view route_path,
                F handler
        ) {
            add_handler(
                    method_t::post,
                    route_path,
                    std::forward<F>(handler)
            );
        }

        template<typename F>
        auto http_post(
                boost::string_view route_path,
                const options &options,
                F handler
        ) {
            add_handler(
                    method_t::post,
                    route_path,
                    options,
                    std::move(handler)
            );
        }

        template<typename F>
        auto http_put(
                boost::string_view route_path,
                F handler
        ) {
            add_handler(
                    method_t::put,
                    route_path,
                    std::move(handler)
            );
        }

        template<typename F>
        auto http_put(
                boost::string_view route_path,
                const options &options,
                F handler
        ) {
            add_handler(
                    method_t::put,
                    route_path,
                    options,
                    std::forward<F>(handler)
            );
        }

    private:

        template<typename F>
        auto add_handler(
                method_t method,
                boost::string_view route_path,
                F handler
        ) {
            add_handler(
                    method,
                    route_path,
                    options{},
                    std::forward<F>(handler)
            );
        }

        template<typename F>
        auto add_handler(
                method_t method,
                boost::string_view route_path,
                const options &options,
                F handler
        ) {
            router_.registration_handler(method, route_path, options, std::forward<F>(handler));
        }

        router router_;
    };

}}