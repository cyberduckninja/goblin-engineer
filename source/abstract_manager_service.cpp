#include <goblin-engineer/abstract_manager_service.hpp>
#include <iostream>

namespace goblin_engineer {

async_policy::async_policy(actor_zeta::supervisor *supervisor,
                           actor_zeta::detail::string_view name)
    : actor_zeta::supervisor(name), executor_(supervisor->executor()) {}

auto async_policy::join(actor_zeta::actor t) -> actor_zeta::actor_address {
  actor_zeta::actor tmp = std::move(t);
  auto current_address = tmp->address();
  actor_zeta::link(*this, current_address);
  storage_.emplace_back(std::move(tmp));
  return current_address;
}

auto async_policy::executor() noexcept -> actor_zeta::executor::abstract_executor & {
  return executor_;
}

auto sync_policy::join(actor_zeta::actor) -> actor_zeta::actor_address {
  return actor_zeta::actor_address();
}

sync_policy::sync_policy(
    actor_zeta::supervisor *,
    actor_zeta::detail::string_view name)
    : actor_zeta::supervisor(name) {}

auto sync_policy::executor() noexcept -> actor_zeta::abstract_executor & {
  std::cerr << "executor"  << std::endl;
  assert(false);
}

    auto async_policy_star::join(actor_zeta::actor tmp) -> actor_zeta::actor_address {
        auto actor = std::move(tmp);
        auto address = actor->address();
        actor_zeta::link(*this, address);
        actor_storage_.emplace_back(std::move(actor));
        return address;
    }

    auto async_policy_star::join(actor_zeta::intrusive_ptr<actor_zeta::supervisor> tmp) -> actor_zeta::actor_address {
        auto supervisor = std::move(tmp);
        auto address = supervisor->address();
        actor_zeta::link(*this, address);
        storage_.emplace_back(std::move(supervisor));
        return address;
    }

    auto async_policy_star::executor() noexcept -> actor_zeta::executor::abstract_executor & {
        return *coordinator_;
    }
}