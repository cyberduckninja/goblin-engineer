#pragma once

#include <goblin-engineer/core/abstract_service.hpp>

#include "router.hpp"

namespace goblin_engineer { namespace http {

        class dispatcher_t final : public abstract_service {
        public:
            template<class T>
            dispatcher_t(actor_zeta::intrusive_ptr<T> ptr,wrapper_router&router,goblin_engineer::string_view name) : abstract_service(ptr, name) {

                router_ = std::move(router.get_router());

                add_handler(
                        "dispatcher",
                        &dispatcher_t::dispatcher
                );
            }

            ~dispatcher_t() override ;

            auto dispatcher(query_context_t&context) -> void ;

        private:
            router router_;

        };

}}