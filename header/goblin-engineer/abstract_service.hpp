#pragma once

#include <goblin-engineer/forward.hpp>
#include <actor-zeta/core.hpp>
#include <actor-zeta/environment.hpp>
#include <actor-zeta/supervisor_heavy.hpp>

namespace goblin_engineer {

    using actor_zeta::actor::basic_async_actor;

    using lite = actor_zeta::supervisor;

    using heavy = actor_zeta::environment::supervisor_heavy;

    struct abstract_service : public basic_async_actor {

        template<std::size_t N>
        abstract_service(dynamic_environment *env, const char(&name)[N]): basic_async_actor(name) {}

        ~abstract_service() override = default;

        virtual void startup(context_t *) = 0;

        virtual void shutdown() = 0;

    };

    struct abstract_manager_service_heavy : public heavy {

        template<std::size_t N>
        abstract_manager_service_heavy(dynamic_environment *env, const char(&name)[N]): heavy(name) {}

        ~abstract_manager_service_heavy() override = default;

        virtual void startup(goblin_engineer::context_t *) = 0;

//        virtual void shutdown() = 0;
    };

    struct abstract_manager_service_lite : public lite {

        template<std::size_t N>
        abstract_manager_service_lite(dynamic_environment *env, const char(&name)[N]): lite(name) {}

        ~abstract_manager_service_lite() override = default;

        virtual void startup(goblin_engineer::context_t *) = 0;

//        virtual void shutdown() = 0;
    };
}

