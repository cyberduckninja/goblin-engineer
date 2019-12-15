#pragma once

#include <string>
#include <iostream>
#include <chrono>
#include <mutex>
#include <spdlog/spdlog.h>
#include <spdlog/async_logger.h>

namespace goblin_engineer {

    using std::chrono::system_clock;
    using std::chrono::duration_cast;
    using std::chrono::milliseconds;
    using std::chrono::seconds;

    class log final {
    public:
        log() = default;

        log(std::shared_ptr<spdlog::async_logger> logger):logger_(logger){}

        ~log() = default;

        auto clone() noexcept -> log {
            return logger_;
        }

        template<typename MSGBuilder>
        void trace(MSGBuilder &&msg_builder) {
            logger_->trace(std::forward<MSGBuilder>(msg_builder));
        }

        template<typename MSGBuilder>
        void info(MSGBuilder &&msg_builder) {
            logger_->info(std::forward<MSGBuilder>(msg_builder));
        }

        template<typename MSGBuilder>
        void warn(MSGBuilder &&msg_builder) {
            logger_->warn(std::forward<MSGBuilder>(msg_builder));
        }

        template<typename MSGBuilder>
        void error(MSGBuilder &&msg_builder) {
            logger_->error(std::forward<MSGBuilder>(msg_builder));
        }

        template<typename MSGBuilder>
        void debug(MSGBuilder &&msg_builder) {
            logger_->debug(std::forward<MSGBuilder>(msg_builder));
        }

    private:
        std::shared_ptr<spdlog::async_logger> logger_;
    };
}