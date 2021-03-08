#pragma once

#include <goblin-engineer/core/abstract_manager_service.hpp>
#include <goblin-engineer/core/abstract_service.hpp>
#include <goblin-engineer/core/forward.hpp>

namespace goblin_engineer {

    template<typename Actor, typename Manager, typename... Args>
    auto make_service(actor_zeta::intrusive_ptr<Manager> &manager, Args &&... args) {
        return manager->join(new Actor(manager, std::forward<Args>(args)...));
    }

    template<
            typename Manager,
            typename... Args
    >
    auto make_manager_service(actor_zeta::intrusive_ptr<Manager> &manager, Args &&... args) {
        actor_zeta::intrusive_ptr<Manager> tmp(
                new Manager(
                        &manager,
                        std::forward<Args>(args)...
                )
        );
        manager->join(tmp);
        return tmp;
    }

    template<typename Manager, typename... Args>
    auto make_manager_service(Args &&... args) -> actor_zeta::intrusive_ptr<Manager>  {
        return actor_zeta::intrusive_ptr<Manager>(
                new Manager(std::forward<Args>(args)...));
    }

} // namespace goblin_engineer