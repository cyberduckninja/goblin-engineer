#pragma once

#include <boost/asio.hpp>

#include <actor-zeta/core.hpp>
#include <goblin-engineer/forward.hpp>

namespace goblin_engineer {  namespace components {

class root_manager final : public actor_zeta::supervisor {
public:
  root_manager(size_t /*num_worker_threads*/, size_t /*max_throughput*/);

  ~root_manager() override;

  void startup();

  void shutdown();

  auto executor() noexcept -> actor_zeta::executor::abstract_executor & override;

  auto join(actor_zeta::actor) -> actor_zeta::actor_address override;

  auto join(actor_zeta::intrusive_ptr<actor_zeta::supervisor>) -> actor_zeta::actor_address;

  auto loop() -> boost::asio::io_context&;

private:

  void enqueue(message, actor_zeta::executor::execution_device *) override;

  auto start() -> std::size_t;

  std::unique_ptr<actor_zeta::executor::abstract_executor> coordinator_;
  std::unique_ptr<boost::asio::io_context> io_context_;
  std::vector<actor_zeta::intrusive_ptr<actor_zeta::supervisor>> storage_;
};


template<
    typename Actor,
    typename Manager,
    typename... Args
>
auto make_service(actor_zeta::intrusive_ptr<Manager>&manager, Args&&... args){
  return manager->join(new Actor(manager,std::forward<Args>(args)...));
}


template<
    typename Manager,
    typename... Args
>
auto make_manager_service(root_manager&app,Args&&... args){
  actor_zeta::intrusive_ptr<Manager> tmp(
      new Manager(
          &app,
          std::forward<Args>(args)...
      )
  );
  app.join(tmp);
  return tmp;
}

}} // namespace goblin_engineer
