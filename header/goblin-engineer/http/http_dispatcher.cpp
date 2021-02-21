#include "http_dispatcher.hpp"

#include "router.hpp"

#include <goblin-engineer.hpp>
#include <goblin-engineer/http/network.hpp>

namespace goblin_engineer { namespace http {


    auto dispatcher_t::dispatcher(query_context&context) -> void {
      router_.invoke(context);
    }

    dispatcher_t::~http_dispatcher_t() =default;

 }}