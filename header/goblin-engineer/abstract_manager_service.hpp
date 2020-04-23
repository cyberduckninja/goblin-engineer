#pragma once

#include <actor-zeta/core.hpp>
#include <goblin-engineer/forward.hpp>


namespace goblin_engineer {

class async_policy : public actor_zeta::supervisor {
public:
  async_policy(actor_zeta::supervisor *, actor_zeta::detail::string_view);

  ~async_policy() override = default;

  auto executor() noexcept -> actor_zeta::executor::abstract_executor & override;

  auto join(actor_zeta::actor) -> actor_zeta::actor_address override;

private:
  actor_zeta::abstract_executor &executor_;
  std::vector<actor_zeta::actor> storage_;
};

class sync_policy : public actor_zeta::supervisor {
public:
  sync_policy(actor_zeta::supervisor *, actor_zeta::detail::string_view);
  ~sync_policy() override = default;
  auto executor() noexcept -> actor_zeta::abstract_executor & override;
  auto join(actor_zeta::actor) -> actor_zeta::actor_address override;
};

template <class BasePolicy>
struct basic_manager_service_t : public BasePolicy {

  template <class... Args>
  basic_manager_service_t(Args &&... args)
      : BasePolicy(std::forward<Args>(args)...) {}
  ~basic_manager_service_t() override = default;
};

using abstract_manager_service = basic_manager_service_t<async_policy>;

} // namespace goblin_engineer