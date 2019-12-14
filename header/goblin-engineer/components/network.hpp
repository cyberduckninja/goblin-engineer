#pragma once

#include <goblin-engineer/forward.hpp>
#include <goblin-engineer/abstract_manager_service.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>

namespace goblin_engineer { namespace components {

    class network_manager_service : public abstract_manager_service {
    public:
        network_manager_service(
                root_manager *env
                , actor_zeta::detail::string_view name
                , std::size_t concurrency_hint
        );

        ~network_manager_service() override;

        auto loop() -> boost::asio::io_context &;

        void enqueue(message msg, actor_zeta::executor::execution_device *) override;

    protected:
        boost::asio::io_context io_context_;
        boost::asio::executor_work_guard<boost::asio::io_context::executor_type> work_;
        boost::thread_group &threadpool_;
    };

}}