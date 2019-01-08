#pragma once

#include <goblin-engineer/forward.hpp>
#include <actor-zeta/actor/basic_actor.hpp>

namespace goblin_engineer {

    using actor_zeta::actor::basic_async_actor;

    struct abstract_service: public basic_async_actor {

        template <std::size_t N>
        abstract_service(actor_zeta::environment::abstract_environment *env,const char(&name)[N]): basic_async_actor(env,name){}

        virtual ~abstract_service() = default;

        static constexpr bool in_plugin = false;

        virtual void startup(goblin_engineer::context_t *) = 0;

        virtual void shutdown() = 0;

        ////service_state state_;
    };

}

