#pragma once
namespace goblin_engineer { namespace components { namespace http_server {

    class multiplexer final {
    public:
        multiplexer() = default;

        ~multiplexer() = default;

        auto port(unsigned short port) -> multiplexer & {
            port_ = port;
            return *this;
        }

        auto port() const -> unsigned short {
            return port_;
        }
/*
    auto address_default_v4() ->boost::asio::ip::tcp {
        return *tcp_.get_ptr();
    }

    auto address_default_v4() -> multiplexer& {
        tcp_ = boost::asio::ip::tcp::v4();
        return *this;
    }

    auto address_default_v6() -> multiplexer& {
        tcp_ = boost::asio::ip::tcp::v6();
        return *this;
    }
*/

    private:
        ///boost::optional<boost::asio::ip::tcp> tcp_;
        unsigned short port_;
    };

}}}