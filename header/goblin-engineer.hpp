#pragma once

#include <goblin-engineer/forward.hpp>
#include <goblin-engineer/dynamic.hpp>
#include <goblin-engineer/abstract_manager_service.hpp>
#include <goblin-engineer/abstract_service.hpp>
#include <goblin-engineer/dynamic_environment.hpp>
#include <actor-zeta/core.hpp>

namespace goblin_engineer {

    using actor_zeta::make_actor;

    template<
            typename Actor,
            typename Supervisor,
            typename... Args
    >
    inline auto make_service(Supervisor* supervisor, Args... args){
        return make_actor<Actor>(supervisor,std::forward<Args>(args)...);
    }

}