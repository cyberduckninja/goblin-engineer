#pragma once

#include <actor-zeta/environment.hpp>
#include <actor-zeta/messaging/message.hpp>

namespace goblin_engineer {

    using abstract_environment = actor_zeta::environment::abstract_environment;

    using message = actor_zeta::messaging::message;

    struct context_t;

    class dynamic_environment;

    class dynamic_config;

    struct abstract_service;

}
