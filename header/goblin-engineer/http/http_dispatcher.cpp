#include "http_dispatcher.hpp"

#include "router.hpp"

#include <goblin-engineer/http/network.hpp>

namespace goblin_engineer { namespace http {

    auto dispatcher_t::dispatcher(query_context_t& context) -> void {
        router_.invoke(context);
    }

    dispatcher_t::~dispatcher_t() = default;

}} // namespace goblin_engineer::http