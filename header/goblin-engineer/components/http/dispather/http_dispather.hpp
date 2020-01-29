#pragma once

#include <goblin-engineer/abstract_service.hpp>
#include <goblin-engineer/components/http/http_server/server.hpp>
#include "router.hpp"

namespace goblin_engineer { namespace components { namespace dispatcher {

        class http_dispatcher final : public abstract_service {
        public:
            http_dispatcher(http_server::server *, dynamic_config &);

            ~http_dispatcher() override = default;

        private:
            router router_;

        };

}}}