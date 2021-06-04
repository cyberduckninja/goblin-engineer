#pragma once

#include <memory>
#include <unordered_map>
#include <vector>

#include <goblin-engineer/http.hpp>

#include <boost/asio/ip/tcp.hpp>
using tcp = boost::asio::ip::tcp;

class http_connection;
using session_type = http_connection;

using addres_book = std::function<goblin_engineer::actor_address(goblin_engineer::string_view)>;

class network_context final {
public:
    using storage_type = std::unordered_map<goblin_engineer::http::session_id, std::shared_ptr<http_connection>>;
    using const_iterator = storage_type::const_iterator;
    using iterator = storage_type::iterator;

    template<class F>
    network_context(F&& f)
        : address_book_(std::forward<F>(f)) {
        self_ = address_book_("self");
    }

    ~network_context();

    template<typename... Args>
    void dispatch(Args... args) {
        actor_zeta::send(address_book_("mq"), self_, "dispatch", std::forward<Args>(args)...);
    }

    session_type& session(network_context*, tcp::socket socket);
    session_type& session(goblin_engineer::http::session_id id);

    auto begin() -> iterator;

    auto end() -> iterator;

    auto begin() const -> const_iterator;

    auto end() const -> const_iterator;

private:
    actor_zeta::actor_address self_;
    addres_book address_book_;
    storage_type sessions_;
};