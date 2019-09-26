#include <goblin-engineer/abstract_service.hpp>
#include <goblin-engineer/dynamic_environment.hpp>

namespace goblin_engineer {

    using abstract_environment = actor_zeta::environment::abstract_environment;

    abstract_manager_service_lite::abstract_manager_service_lite(dynamic_environment *env, char const *name) : lite(name) {}

    auto abstract_manager_service_lite::join(actor_zeta::supervisor &) -> void {}

    auto abstract_manager_service_lite::join(actor_zeta::actor::base_actor *t) -> actor_zeta::actor::actor_address {}

    auto abstract_manager_service_lite::broadcast(message) -> bool {}

    auto abstract_manager_service_lite::executor() noexcept -> actor_zeta::executor::abstract_executor & {}

    auto abstract_manager_service_lite::startup() noexcept -> void {}

    auto abstract_manager_service_lite::shutdown() noexcept -> void {}

    void abstract_manager_service_lite::enqueue(message, actor_zeta::executor::execution_device *) {}
}

