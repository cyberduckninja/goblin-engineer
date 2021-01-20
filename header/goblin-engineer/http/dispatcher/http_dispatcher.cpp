#include "http_dispatcher.hpp"
#include "router.hpp"

#include <unordered_map>


#include <goblin-engineer.hpp>
#include <goblin-engineer/http/detail/network.hpp>

namespace goblin_engineer { namespace components { namespace dispatcher {


    auto http_dispatcher::dispatcher(detail::query_context&context) -> void {
      router_.invoke(context);
    }

    http_dispatcher::~http_dispatcher() =default;

 }}}