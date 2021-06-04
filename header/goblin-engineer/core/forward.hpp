#pragma once

#include <actor-zeta/actor-zeta.hpp>

namespace goblin_engineer {

    struct abstract_service;

    using actor_zeta::abstract_executor;
    using actor_zeta::actor;
    using actor_zeta::actor_address;
    using actor_zeta::context;
    using actor_zeta::execution_device;
    using actor_zeta::intrusive_ptr;
    using actor_zeta::link;
    using actor_zeta::make_message;
    using actor_zeta::message_ptr;
    using actor_zeta::send;
    using actor_zeta::supervisor_t;
    using actor_zeta::base::message;
    using actor_zeta::detail::string_view;
} // namespace goblin_engineer
