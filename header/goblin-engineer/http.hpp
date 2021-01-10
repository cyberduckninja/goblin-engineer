#pragma once

#include <goblin-engineer/http/dispatcher/http_dispatcher.hpp>
#include <goblin-engineer/http/detail/network.hpp>

namespace goblin_engineer {  namespace http {

    using router =  components::dispatcher::wrapper_router;
    using components::dispatcher::http_dispatcher;
    using components::detail::query_context;
    using request_type = components::detail::request_type;
    using response_type = components::detail::response_type;

}}



