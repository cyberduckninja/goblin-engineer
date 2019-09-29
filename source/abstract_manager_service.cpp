#include <goblin-engineer/abstract_manager_service.hpp>
#include <goblin-engineer/dynamic_environment.hpp>

namespace goblin_engineer {

    using abstract_environment = actor_zeta::environment::abstract_environment;

    template <class Actor >
    inline auto send(Actor& actor,message msg){
        actor->enqueue(std::move(msg));
    }


    abstract_manager_service::abstract_manager_service(dynamic_environment *env, actor_zeta::detail::string_view name)
        : lite(name)
        , executor_(env->executor())
        , io_context_(env->main_loop())
        , thread_group_(env->background())
        {
    }

    auto abstract_manager_service::join(base_actor *t) -> actor_zeta::actor_address {
        auto address = t->address();
        storage_.emplace_back(t);
        return address;
    }

    auto abstract_manager_service::broadcast(message msg) -> bool {
        for(auto&i:storage_){
            send(i,msg.clone());
        }
        return true;
    }

    auto abstract_manager_service::executor() noexcept -> actor_zeta::executor::abstract_executor & {
        return executor_;
    }

    void abstract_manager_service::enqueue(message, actor_zeta::executor::execution_device *) {

    }

    auto abstract_manager_service::loop() -> boost::asio::io_context& {
        return io_context_;
    }

    auto abstract_manager_service::thread_pool() -> boost::thread_group & {
        return thread_group_;
    }
}

