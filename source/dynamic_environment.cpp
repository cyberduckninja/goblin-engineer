#include <goblin-engineer/dynamic_environment.hpp>

#include <forward_list>
#include <iostream>

#include <boost/asio.hpp>

#include <goblin-engineer/dynamic.hpp>
#include <goblin-engineer/abstract_service.hpp>


namespace goblin_engineer {

    void dynamic_environment::shutdown() {
        io_context_->stop();
    }

    void dynamic_environment::startup() {

        start();

        shutdown();
    }

    void dynamic_environment::initialize() {}

    dynamic_environment::dynamic_environment(dynamic_config &&f)
        : coordinator_(new actor_zeta::executor::executor_t<actor_zeta::executor::work_sharing>(1, 1000))
        , io_context_(new boost::asio::io_context)
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

    dynamic_environment::~dynamic_environment() {
        io_context_->stopped();
        std::cerr << "~goblin-engineer" << std::endl;
    }

    std::size_t dynamic_environment::start() {
        executor().start();
        return io_context_->run();
    }

    auto dynamic_environment::executor() -> actor_zeta::executor::abstract_executor & {
        return *coordinator_;
    }

    auto dynamic_environment:: configuration() -> dynamic_config & {
        return configuration_;
    }

    auto dynamic_environment::environment() -> goblin_engineer::dynamic_environment  * {
        return static_cast<goblin_engineer::dynamic_environment  *>(this);
    }
}