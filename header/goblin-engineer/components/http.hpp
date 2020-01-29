#pragma once

#include <goblin-engineer/components/http/dispather/http_dispather.hpp>
#include <goblin-engineer/components/http/dispather/router.hpp>
#include <goblin-engineer/components/http/http_server/server.hpp>

namespace goblin_engineer { namespace components { namespace http {

    using router =  dispatcher::wrapper_router;
    using dispatcher::http_dispatcher;
    using http_server::server;

    class  multiplexer final {
    public:
        multiplexer() = default;
        ~multiplexer() = default;

        auto  port( unsigned short port ) -> multiplexer& {
            port_ = port;
            return *this;
        }

        auto address_default_v4() -> multiplexer& {
            tcp_ = boost::asio::ip::tcp::v4();
            return *this;
        }

        auto address_default_v6() -> multiplexer& {
            tcp_ = boost::asio::ip::tcp::v6();
            return *this;
        }


    private:
        boost::asio::ip::tcp tcp_;
        unsigned short port_;
    };


}}}



