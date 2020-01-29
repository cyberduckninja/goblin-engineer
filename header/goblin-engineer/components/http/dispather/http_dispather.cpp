
#include "http_dispather.hpp"
#include "router.hpp"

#include <unordered_map>


#include <goblin-engineer.hpp>
#include <goblin-engineer/components/http/http_server/server.hpp>
#include <goblin-engineer/components/http/detail/network.hpp>

namespace goblin_engineer { namespace components { namespace dispatcher {


    http_dispatcher::http_dispatcher(http_server::server *ptr, dynamic_config &) : abstract_service(ptr, "http_dispatcher") {

        wrapper_router wrapper_router_;

        router_ = wrapper_router_.get_router();

        add_handler(
                "dispatcher",
                [this](actor_zeta::actor::context &, detail::query_context&context){
                    router_.invoke(context);
                }
        );
    }

}}}