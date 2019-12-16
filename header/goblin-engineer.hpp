#pragma once

#include <actor-zeta/core.hpp>

#include <goblin-engineer/abstract_manager_service.hpp>
#include <goblin-engineer/abstract_service.hpp>
#include <goblin-engineer/dynamic.hpp>
#include <goblin-engineer/forward.hpp>
#include <goblin-engineer/log.hpp>
#include <goblin-engineer/root_manager.hpp>

namespace goblin_engineer {

    using actor_zeta::make_actor;

    template<
            typename Actor,
            typename Supervisor,
            typename... Args
    >
    inline auto make_service(Supervisor* supervisor, Args&&... args){
        return make_actor<Actor>(supervisor,std::forward<Args>(args)...);
    }

}