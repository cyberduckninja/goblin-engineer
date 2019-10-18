#include <goblin-engineer/root_manager.hpp>

#include <forward_list>
#include <iostream>

#include <boost/asio.hpp>

#include <goblin-engineer/dynamic.hpp>
#include <goblin-engineer/abstract_service.hpp>


namespace goblin_engineer {

    void root_manager::shutdown() {
        io_context_->stop();
    }

    void root_manager::startup() {

        start();

        shutdown();
    }

    void root_manager::initialize() {}

    root_manager::root_manager(dynamic_config &&f)
        : supervisor("root_manager")
        , coordinator_(new actor_zeta::executor::executor_t<actor_zeta::executor::work_sharing>(1, 1000))
        , io_context_(std::make_unique<boost::asio::io_context>())
        , background_(std::make_unique<boost::thread_group>())
        , configuration_ (std::move(f))
        {

        std::shared_ptr<boost::asio::signal_set> sigint_set(new boost::asio::signal_set(*io_context_, SIGINT, SIGTERM));
        sigint_set->async_wait(
                [sigint_set, this](const boost::system::error_code &/*err*/, int /*num*/) {
                    shutdown();
                    sigint_set->cancel();
                }
        );

    }

    root_manager::~root_manager() {
        background_->join_all();
        std::cerr << "~goblin-engineer" << std::endl;
    }

    std::size_t root_manager::start() {
        executor().start();
        return io_context_->run();
    }

    auto root_manager::executor() noexcept -> actor_zeta::executor::abstract_executor & {
        return *coordinator_;
    }

    auto root_manager:: configuration() -> dynamic_config & {
        return configuration_;
    }

    auto root_manager::environment() -> root_manager* {
        return static_cast<root_manager*>(this);
    }

    auto root_manager::background() const -> boost::thread_group & {
        return *background_;
    }

    auto root_manager::join(actor_zeta::actor::base_actor *) -> actor_zeta::actor::actor_address {
        return actor_zeta::actor::actor_address();
    }

    void root_manager::enqueue(message, actor_zeta::executor::execution_device *) {}

    auto root_manager::broadcast(message) -> bool {return  true; }

    auto root_manager::join(actor_zeta::intrusive_ptr<actor_zeta::supervisor> tmp) -> actor_zeta::actor::actor_address {
        auto addresses =  tmp->address();
        storage_.emplace_back(tmp);
        return addresses;
    }
}