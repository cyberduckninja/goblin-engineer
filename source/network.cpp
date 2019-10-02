#include <goblin-engineer/components/network.hpp>

#include <goblin-engineer/dynamic_environment.hpp>

namespace goblin_engineer { namespace components {

        network_manager_service::network_manager_service(
                goblin_engineer::dynamic_environment *env,
                actor_zeta::detail::string_view name,
                std::size_t concurrency_hint
        )
        : abstract_manager_service(env, name)
        , io_context_(concurrency_hint)
        , work_(boost::asio::make_work_guard(io_context_))
        , threadpool_(env->background()) {
            threadpool_.create_thread(
                    [&]() {
                        io_context_.run();
                    }
            );
        }

        network_manager_service::~network_manager_service() {
            io_context_.stop();
        }

        auto network_manager_service::loop() -> boost::asio::io_context & {
            return io_context_;
        }

        void network_manager_service::enqueue(message msg, actor_zeta::executor::execution_device *) {
            actor_zeta::context tmp(this,std::move(msg));
            dispatch().execute(tmp);
        }

}}