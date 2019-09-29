#pragma once

#include <goblin-engineer/abstract_manager_service.hpp>

#include <boost/asio.hpp>

namespace goblin_engineer { namespace components{

    class network_manager_service : public abstract_manager_service {
    public:

        rm -rf .git/modules/a/submodulenetwork_manager_service(dynamic_environment* env,actor_zeta::detail::string_view name)
            :abstract_manager_service(env, name )
            {

        }

        auto loop() -> boost::asio::io_context& {
            return io_context_;
        }

    protected:
        boost::asio::io_context io_context_;
    };

}}