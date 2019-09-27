#pragma once

#include <cassert> /// HACK

#include <goblin-engineer/forward.hpp>
#include <actor-zeta/core.hpp>

namespace goblin_engineer {

    using actor_zeta::actor::basic_async_actor;

    struct abstract_service : public basic_async_actor {

        template<std::size_t N>
        abstract_service(abstract_manager_service_lite* supervisor, const char(&name)[N]): basic_async_actor(supervisor,name) {}

        ~abstract_service() override = default;

        virtual void startup(context_t *) = 0;

        virtual void shutdown() = 0;
    };


}

