#include <goblin-engineer/abstract_manager_service.hpp>
#include <iostream>

namespace goblin_engineer {

    auto sync_policy::join(actor_zeta::actor) -> actor_zeta::actor_address {
        return actor_zeta::actor_address();
    }

    sync_policy::sync_policy(
            actor_zeta::supervisor *,
            actor_zeta::detail::string_view name)
            : actor_zeta::supervisor(name) {}

    auto sync_policy::executor() noexcept -> actor_zeta::abstract_executor & {
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

    auto async_policy::join(actor_zeta::intrusive_ptr<actor_zeta::supervisor> tmp) -> actor_zeta::actor_address {
        auto supervisor = std::move(tmp);
        auto address = supervisor->address();
        actor_zeta::link(*this, address);
        storage_.emplace_back(std::move(supervisor));
        return address;
    }

    auto async_policy::executor() noexcept -> actor_zeta::executor::abstract_executor & {
        return *coordinator_;
    }

    async_policy::async_policy(actor_zeta::abstract_executor *executor, actor_zeta::detail::string_view view)
            : actor_zeta::supervisor(view), coordinator_(executor) {}


    auto async_policy_lite::join(goblin_engineer::actor tmp) -> goblin_engineer::actor_address {
        auto actor = std::move(tmp);
        auto address = actor->address();
        goblin_engineer::link(*this, address);
        return address;
    }

    auto async_policy_lite::join(
            goblin_engineer::intrusive_ptr<goblin_engineer::supervisor> tmp) -> goblin_engineer::actor_address {
        auto supervisor = std::move(tmp);
        auto address = supervisor->address();
        goblin_engineer::link(*this, address);
        return address;
    }

    auto async_policy_lite::executor() noexcept -> goblin_engineer::abstract_executor & {
        return *coordinator_;
    }

    async_policy_lite::async_policy_lite(goblin_engineer::abstract_executor *executor, goblin_engineer::string_view view)
            : goblin_engineer::supervisor(view), coordinator_(executor) {}

}