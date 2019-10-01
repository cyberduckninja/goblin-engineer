#pragma once

#include <deque>

#include <boost/asio.hpp>
#include <boost/thread.hpp>

#include <goblin-engineer/forward.hpp>
#include <actor-zeta/core.hpp>

namespace goblin_engineer {

    using lite = actor_zeta::supervisor;

    struct abstract_manager_service : public lite {

        using actor_zeta::supervisor::join;

        abstract_manager_service(dynamic_environment *, actor_zeta::detail::string_view );

        ~abstract_manager_service() override = default;

        auto executor() noexcept -> actor_zeta::executor::abstract_executor & override;

        auto broadcast(message) -> bool override;

        auto join(base_actor *t) -> actor_zeta::actor_address override;

    private:
        actor_zeta::executor::abstract_executor & executor_;
        std::vector<actor_zeta::actor::actor> storage_;
    };

}