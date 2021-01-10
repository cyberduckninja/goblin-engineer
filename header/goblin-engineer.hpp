#pragma once

#include <goblin-engineer/abstract_manager_service.hpp>
#include <goblin-engineer/abstract_service.hpp>
#include <goblin-engineer/forward.hpp>
#include <actor-zeta/core.hpp>

namespace goblin_engineer {

    using actor_zeta::send;
    using actor_zeta::supervisor;
    using actor_zeta::actor_address;
    using actor_zeta::detail::string_view;
    using actor_zeta::execution_device;
    using actor_zeta::context;
    using actor_zeta::intrusive_ptr;
    using actor_zeta::abstract_executor;
    using actor_zeta::message;
    using actor_zeta::link;
    using actor_zeta::actor;
    ///using actor_zeta::make_message;

    template<typename Actor, typename Manager, typename... Args>
    auto make_service(actor_zeta::intrusive_ptr<Manager> &manager, Args &&... args) {
        return manager->join(new Actor(manager, std::forward<Args>(args)...));
    }

    template<
            typename Root,
            typename Manager,
            typename... Args
    >
    auto make_manager_service(Root &app, Args &&... args) {
        actor_zeta::intrusive_ptr<Manager> tmp(
                new Manager(
                        &app,
                        std::forward<Args>(args)...
                )
        );
        app.join(tmp);
        return tmp;
    }

    template<typename Manager, typename... Args>
    auto make_manager_service(Args &&... args) -> actor_zeta::intrusive_ptr<Manager>  {
        return actor_zeta::intrusive_ptr<Manager>(
                new Manager(std::forward<Args>(args)...));
    }

} // namespace goblin_engineer