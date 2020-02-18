#include <goblin-engineer/root_manager.hpp>

#include <forward_list>
#include <iostream>

#include <goblin-engineer/dynamic.hpp>
#include <goblin-engineer/abstract_service.hpp>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include "spdlog/async.h"

namespace goblin_engineer {
constexpr const static char * log_name = "root_log";
    void root_manager::shutdown() {
        background_->join_all();
        io_context_->stop();
        log_.info("root_manager::shutdown");
    }

    void root_manager::startup() {
        start();
        log_.info("root_manager::startup");
        shutdown();
    }

    void root_manager::initialize() {}

    root_manager::root_manager(dynamic_config f)
        : supervisor("root_manager")
        , configuration_ (std::move(f))
        , coordinator_(new actor_zeta::executor::executor_t<actor_zeta::executor::work_sharing>(1, 1000))
        , io_context_(std::make_unique<boost::asio::io_context>())
        , background_(std::make_unique<boost::thread_group>())
        , log_()
        {

            spdlog::init_thread_pool(8192, 1);
            auto stdout_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
            auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/log.txt", true);
            std::vector<spdlog::sink_ptr> sinks{stdout_sink, file_sink};
            auto logger = std::make_shared<spdlog::async_logger>(
                    log_name,
                    sinks.begin(),sinks.end(),
                    spdlog::thread_pool(),
                    spdlog::async_overflow_policy::block
            );

            spdlog::register_logger(logger);

            ///spdlog::set_pattern("[%H:%M:%S %z] [%^%L%$] [thread %t] %v");

            log_.context(logger);

            signal_set_ = std::make_shared<boost::asio::signal_set>(*io_context_, SIGINT, SIGTERM);
            signal_set_->async_wait(
                    [this](const boost::system::error_code &/*err*/, int /*num*/) {
                        log_.info("root_manager::root_manager signal_set");
                        shutdown();
                        signal_set_->cancel();
                    }
            );

            log_.info("root_manager");

    }

    root_manager::~root_manager() {
        log_.info("~root_manager");
        ///spdlog::apply_all([&](std::shared_ptr<spdlog::logger> l) { l->info("root_manager::shutdown"); });
        spdlog::shutdown();
    }

    std::size_t root_manager::start() {
        log_.info("root_manager::start");
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

    void root_manager::enqueue(message msg, actor_zeta::executor::execution_device *) {
        boost::asio::post(
                *io_context_,
                [this, msg = std::move(msg)]() mutable {
                    actor_zeta::actor::context tmp(this, std::move(msg));
                    dispatch().execute(tmp);
                }
        );
    }

    auto root_manager::broadcast(message msg) -> bool {
        auto msg_ = std::move(msg);
        for (auto &i:storage_) {
            i->enqueue(msg_.clone());
        }
        return true;
    }

    auto root_manager::join(actor_zeta::intrusive_ptr<actor_zeta::supervisor> tmp) -> actor_zeta::actor::actor_address {
        auto supervisor = std::move(tmp);
        auto address =  supervisor->address();
        actor_zeta::link(this,address);
        storage_.emplace_back(std::move(supervisor));
        return address;
    }

    auto root_manager::logger() -> log {
        return log_.clone();
    }
}