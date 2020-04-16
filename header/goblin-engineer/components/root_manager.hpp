#pragma once

#include <boost/asio.hpp>

#include <actor-zeta/core.hpp>
#include <goblin-engineer/detail/dynamic.hpp>
#include <goblin-engineer/forward.hpp>

namespace goblin_engineer {  namespace components {

class root_manager final : public actor_zeta::supervisor {
public:
  root_manager(size_t /*num_worker_threads*/, size_t /*max_throughput*/);

  ~root_manager() override = default;

  template <class Manager, typename... Args>
  auto add_manager_service(Args &&... args) {
    actor_zeta::intrusive_ptr<Manager> tmp(
        new Manager(
            environment(),
            std::forward<Args>(args)...
        )
    );
    join(tmp);
    return tmp;
  }

  void startup();

  void shutdown();

  auto executor() noexcept -> actor_zeta::executor::abstract_executor & override;

  auto join(actor_zeta::actor) -> actor_zeta::actor_address override;

private:

  auto join(actor_zeta::intrusive_ptr<actor_zeta::supervisor>) -> actor_zeta::actor_address;

  void enqueue(message, actor_zeta::executor::execution_device *) override;

  auto environment() -> root_manager *;

  auto start() -> std::size_t;

  std::unique_ptr<actor_zeta::executor::abstract_executor> coordinator_;
  std::unique_ptr<boost::asio::io_context> io_context_;
  std::vector<actor_zeta::intrusive_ptr<actor_zeta::supervisor>> storage_;
};


template<
    typename Actor,
    typename... Args
>
auto make_service(root_manager&app, Args&&... args){
  return app.join(new Actor(std::forward<Args>(args)...));
}


template<
    typename Actor,
    typename... Args
>
auto make_manager_service(root_manager&app,Args&&... args){
  return app.add_manager_service<Actor>(std::forward<Args>(args)...);
}

}} // namespace goblin_engineer
