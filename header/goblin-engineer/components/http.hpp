#pragma once

#include <goblin-engineer/components/http/dispatcher/http_dispatcher.hpp>
#include <goblin-engineer/components/http/http_server/server.hpp>
#include <goblin-engineer/components/http/detail/network.hpp>

namespace goblin_engineer { namespace components { namespace http {

    using router =  dispatcher::wrapper_router;
    using dispatcher::http_dispatcher;
    using http_server::server;
    using detail::query_context;

}}}



