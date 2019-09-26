#pragma once

#include <cassert> /// HACK

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

    struct abstract_manager_service_lite : public lite {

        abstract_manager_service_lite(dynamic_environment *env, char const*name);

        ~abstract_manager_service_lite() override = default;

        virtual void startup(goblin_engineer::context_t *) = 0;

        void enqueue(message, actor_zeta::executor::execution_device *) override;

        auto shutdown() noexcept -> void override;

        auto startup() noexcept -> void override;

        auto executor() noexcept -> actor_zeta::executor::abstract_executor& override;

        auto broadcast(message) -> bool override;

        auto join(base_actor *t) -> actor_zeta::actor::actor_address override;

        auto join(supervisor &) -> void override;

    };
}

