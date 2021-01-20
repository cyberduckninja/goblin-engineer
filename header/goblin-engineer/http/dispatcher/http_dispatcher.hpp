#pragma once

#include <goblin-engineer/abstract_service.hpp>

#include "router.hpp"

namespace goblin_engineer { namespace components { namespace dispatcher {

        class http_dispatcher final : public abstract_service {
        public:
            template<class T>
            http_dispatcher(actor_zeta::intrusive_ptr<T> ptr,dispatcher::wrapper_router&router) : abstract_service(ptr, "http_dispatcher") {

                router_ = std::move(router.get_router());

                add_handler(
                        "dispatcher",
                        &http_dispatcher::dispatcher
                );
            }

            ~http_dispatcher() override ;

            auto dispatcher(detail::query_context&context) -> void ;

        private:
            router router_;

        };

}}}