#pragma once

#include <goblin-engineer/abstract_manager_service.hpp>

#include <boost/asio.hpp>
#include <iostream>

namespace goblin_engineer { namespace components {

    class network_manager_service : public abstract_manager_service {
    public:
        network_manager_service(
                dynamic_environment *env
                , actor_zeta::detail::string_view name
                , std::size_t concurrency_hint
        )
            : abstract_manager_service(env, name)
            , io_context_(concurrency_hint)
            , work_(boost::asio::make_work_guard(io_context_))
            , threadpool_(env->background())
        {
            threadpool_.create_thread(
                    [&]() {
                        io_context_.run();
                    }
            );
        }

        ~network_manager_service() override {
            io_context_.stop();
        }

        auto loop() -> boost::asio::io_context & {
            return io_context_;
        }

    protected:
        boost::asio::io_context io_context_;
        boost::asio::executor_work_guard<boost::asio::io_context::executor_type> work_;
        boost::thread_group &threadpool_;
    };

}}