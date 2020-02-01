
#include "http_dispather.hpp"
#include "router.hpp"

#include <unordered_map>


#include <goblin-engineer.hpp>
#include <goblin-engineer/components/http/http_server/server.hpp>
#include <goblin-engineer/components/http/detail/network.hpp>

namespace goblin_engineer { namespace components { namespace dispatcher {


    http_dispatcher::http_dispatcher(actor_zeta::intrusive_ptr<http_server::server> ptr, dynamic_config &,dispatcher::wrapper_router&router) : abstract_service(ptr, "http_dispatcher") {

        router_ = std::move(router.get_router());

        add_handler(
                "dispatcher",
                [this](actor_zeta::actor::context &, detail::query_context&context){
                    router_.invoke(context);
                }
        );
    }

}}}