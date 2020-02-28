#pragma once

#include <goblin-engineer/abstract_service.hpp>
#include <goblin-engineer/components/http/http_server/server.hpp>
#include "router.hpp"

namespace goblin_engineer { namespace components { namespace dispatcher {

        class http_dispatcher final : public abstract_service {
        public:
            http_dispatcher(actor_zeta::intrusive_ptr<http_server::server>, dynamic_config &, dispatcher::wrapper_router&);

            ~http_dispatcher() override = default;

        private:
            router router_;

        };

}}}