#pragma once

#include <boost/optional.hpp>

#include <goblin-engineer/components/http/dispather/http_dispather.hpp>
#include <goblin-engineer/components/http/dispather/router.hpp>
#include <goblin-engineer/components/http/http_server/server.hpp>
#include <goblin-engineer/components/http/detail/network.hpp>

namespace goblin_engineer { namespace components { namespace http {

    using router =  dispatcher::wrapper_router;
    using dispatcher::http_dispatcher;
    using http_server::server;
    using detail::query_context;

    class  multiplexer final {
    public:
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
        boost::optional<boost::asio::ip::tcp> tcp_;
        unsigned short port_;
    };


}}}



