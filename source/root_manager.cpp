#include <goblin-engineer/components/root_manager.hpp>

#include <forward_list>
#include <iostream>

#include <goblin-engineer/abstract_service.hpp>
#include <goblin-engineer/detail/dynamic.hpp>

namespace goblin_engineer {  namespace components {

    void root_manager::shutdown() {
        io_context_->stop();
    }

    void root_manager::startup() {

        start();

        shutdown();
    }

    root_manager::root_manager(size_t num_worker_threads, size_t max_throughput_param)
        : supervisor("root_manager")
        , coordinator_(new actor_zeta::executor_t<actor_zeta::work_sharing>(num_worker_threads, max_throughput_param))
        , io_context_(std::make_unique<boost::asio::io_context>()){}

    std::size_t root_manager::start() {
        executor().start();
        return io_context_->run();
    }

    auto root_manager::executor() noexcept -> actor_zeta::abstract_executor & {
        return *coordinator_;
    }

    auto root_manager::environment() -> root_manager* {
        return static_cast<root_manager*>(this);
    }

    auto root_manager::join(actor_zeta::actor ) -> actor_zeta::actor_address {
        return actor_zeta::actor_address();
    }

    void root_manager::enqueue(message msg, actor_zeta::execution_device *) {
        boost::asio::post(
                *io_context_,
                [this, msg = std::move(msg)]() mutable {
                    set_current_message(std::move(msg));
                    dispatch().execute(*this);
                }
        );
    }


    auto root_manager::join(actor_zeta::intrusive_ptr<actor_zeta::supervisor> tmp) -> actor_zeta::actor_address {
        auto supervisor = std::move(tmp);
        auto address =  supervisor->address();
        actor_zeta::link(*this,address);
        storage_.emplace_back(std::move(supervisor));
        return address;
    }

}}