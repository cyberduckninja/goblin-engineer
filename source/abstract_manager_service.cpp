#include <goblin-engineer/core/abstract_manager_service.hpp>
#include <iostream>

namespace goblin_engineer {

    auto sync_policy::join(actor_zeta::actor) -> actor_zeta::actor_address {
        return actor_zeta::actor_address();
    }

    sync_policy::sync_policy(
            goblin_engineer::intrusive_ptr<goblin_engineer::supervisor_t>,
            goblin_engineer::string_view name)
            : actor_zeta::supervisor_t(name) {}

    auto sync_policy::executor() noexcept -> actor_zeta::abstract_executor * {
        std::cerr << "executor" << std::endl;
        assert(false);
    }

    auto async_policy::join(actor_zeta::actor tmp) -> actor_zeta::actor_address {
        auto actor = std::move(tmp);
        auto address = actor->address();
        actor_zeta::link(*this, address);
        actor_storage_.emplace_back(std::move(actor));
        return address;
    }

    auto async_policy::join(actor_zeta::intrusive_ptr<actor_zeta::supervisor_t> tmp) -> actor_zeta::actor_address {
        auto supervisor = std::move(tmp);
        auto address = supervisor->address();
        actor_zeta::link(*this, address);
        storage_.emplace_back(std::move(supervisor));
        return address;
    }


    async_policy::async_policy(goblin_engineer::intrusive_ptr<goblin_engineer::supervisor_t>, goblin_engineer::string_view view)
        :actor_zeta::supervisor_t(view) {

    }

    async_policy::async_policy(goblin_engineer::string_view view) : supervisor_t(view) {

    }


    auto async_policy_lite::join(goblin_engineer::actor tmp) -> goblin_engineer::actor_address {
        auto actor = std::move(tmp);
        auto address = actor->address();
        goblin_engineer::link(*this, address);
        return address;
    }

    auto async_policy_lite::join(
            goblin_engineer::intrusive_ptr<goblin_engineer::supervisor_t> tmp) -> goblin_engineer::actor_address {
        auto supervisor = std::move(tmp);
        auto address = supervisor->address();
        goblin_engineer::link(*this, address);
        return address;
    }

}