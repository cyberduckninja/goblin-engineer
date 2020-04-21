#pragma once

#include <functional>
#include <goblin-engineer/components/http/detail/network.hpp>

namespace goblin_engineer { namespace components { namespace http_server {

    using helper_write_f_t = std::function<void(detail::request_type && , std::size_t)>;

    class identifier_connection final {

    };

using port_t =  unsigned short int;

}}}