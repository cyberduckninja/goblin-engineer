#pragma once

#include <goblin-engineer/forward.hpp>
#include <goblin-engineer/dynamic.hpp>
#include <goblin-engineer/abstract_manager_service.hpp>
#include <goblin-engineer/abstract_service.hpp>
#include <goblin-engineer/root_manager.hpp>
#include <actor-zeta/core.hpp>

namespace goblin_engineer {

    using actor_zeta::make_actor;

    template<
            typename Actor,
            typename Supervisor,
            typename... Args
    >
    auto make_service(actor_zeta::intrusive_ptr<Supervisor>& supervisor, Args&&... args){
        return make_actor<Actor>(supervisor.get(),std::forward<Args>(args)...);
    }

    template<
            typename Actor,
            typename... Args
    >
    auto make_manager_service(root_manager&app,Args&&... args){
        return app.add_manager_service<Actor>(std::forward<Args>(args)...);
    }

}