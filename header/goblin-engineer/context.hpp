#pragma once

#include <boost/asio.hpp>
#include <boost/thread.hpp>

#include <goblin-engineer/forward.hpp>
#include <goblin-engineer/dynamic.hpp>

namespace goblin_engineer {

    struct context_t {

        virtual boost::asio::io_context &main_loop() const = 0;

        virtual boost::thread_group &background()    const = 0;

        virtual ~context_t() = default;
    };

}
