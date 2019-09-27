#pragma once

#include <cassert> /// HACK

#include <goblin-engineer/forward.hpp>
#include <actor-zeta/core.hpp>

namespace goblin_engineer {

    using lite = actor_zeta::supervisor;

    struct abstract_manager_service_lite : public lite {

        using actor_zeta::supervisor::join;

        abstract_manager_service_lite(dynamic_environment *env, char const *name);

        ~abstract_manager_service_lite() override = default;

        virtual void startup(goblin_engineer::context_t *) = 0;

        void enqueue(message, actor_zeta::executor::execution_device *) override;

        auto shutdown() noexcept -> void override;

        auto startup() noexcept -> void override;

        auto executor() noexcept -> actor_zeta::executor::abstract_executor & override;

        auto broadcast(message) -> bool override;

        auto join(base_actor *t) -> actor_zeta::actor::actor_address override;

        auto join(supervisor &) -> void override;
    };

}