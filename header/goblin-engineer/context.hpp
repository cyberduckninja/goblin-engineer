#pragma once

#include <boost/asio.hpp>
#include <boost/thread.hpp>

#include <goblin-engineer/forward.hpp>
#include <goblin-engineer/dynamic.hpp>

namespace goblin_engineer {

    struct context_t {

        virtual auto main_loop() const ->  boost::asio::io_context & = 0;

        virtual auto background() const ->  boost::thread_group & = 0;

        virtual auto config() -> dynamic_config& = 0;

        virtual ~context_t() = default;
    };

}
