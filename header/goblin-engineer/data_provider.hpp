#pragma once

#include <actor-zeta/actor/local_actor.hpp>
#include <goblin-engineer/context.hpp>

namespace goblin_engineer {

    struct data_provider : public actor_zeta::actor::local_actor {

        data_provider(actor_zeta::environment::abstract_environment *,  const std::string &);

        ~data_provider() override = default;

        virtual void startup(goblin_engineer::context_t *) = 0;

        virtual void shutdown() = 0;

        bool send(message, actor_zeta::executor::execution_device *) override;

        void launch(actor_zeta::executor::execution_device *, bool) override;

        bool send(message ) override;

    };

}

