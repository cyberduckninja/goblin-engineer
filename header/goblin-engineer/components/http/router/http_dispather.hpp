#pragma once

#include <goblin-engineer/components/http/router/router.hpp>
#include <goblin-engineer/abstract_service.hpp>

namespace goblin_engineer { namespace components { namespace http {

        class http_dispatcher final
                : public goblin_engineer::abstract_service,
                : public router {
        public:
            http_dispatcher(components::server *,goblin_engineer::dynamic_config&);

            ~http_dispatcher() override = default;

        };
}}}