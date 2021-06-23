#pragma once

#include <goblin-engineer/core/forward.hpp>

namespace goblin_engineer {

    using actor_zeta::basic_async_actor;

    struct abstract_service : public basic_async_actor {
        /*
        *   makes a service and links it with a supervisor
        *   @param supervisor context for a service
        *   @param name name for a service, use std::move on it
        * */
        template<class Supervisor>
        abstract_service(actor_zeta::intrusive_ptr<Supervisor> supervisor, std::string name)
            : basic_async_actor(supervisor.get(), std::move(name)) {}

        ~abstract_service() override = default;
    };

} // namespace goblin_engineer
