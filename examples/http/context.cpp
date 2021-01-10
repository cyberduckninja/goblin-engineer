#include "context.hpp"

#include <boost/asio/ip/tcp.hpp>

namespace net = boost::asio;
using tcp = boost::asio::ip::tcp;

#include "http_connection.hpp"


session_type &network_context::session(network_context* ctx,tcp::socket &&socket) {
    auto session = std::make_shared<session_type>(ctx,std::move(socket));
    sessions_.emplace(reinterpret_cast<std::uintptr_t>(session.get()), session);
    return *session;
}

session_type &network_context::session(session_id id) {
    return *sessions_.at(id);
}

auto network_context::begin() -> network_context::iterator {
    return sessions_.begin();
}

auto network_context::end() -> network_context::iterator {
    return sessions_.end();
}

auto network_context::begin() const -> network_context::const_iterator {
    return sessions_.begin();
}

auto network_context::end() const -> network_context::const_iterator {
    return sessions_.end();
}

network_context::~network_context() =default;
