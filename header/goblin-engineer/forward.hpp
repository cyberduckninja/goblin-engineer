#pragma once

#include <actor-zeta/environment/environment.hpp>
#include <actor-zeta/environment/group.hpp>
#include <actor-zeta/messaging/message.hpp>

namespace goblin_engineer {

    using abstract_environment = actor_zeta::environment::abstract_environment;

    using service = actor_zeta::environment::group;

    using message = actor_zeta::messaging::message;

    struct context_t;

    class dynamic_environment;

    class dynamic_config;

    class plugin;

    struct abstract_plugin;

    struct abstract_service;

    struct data_provider;

    struct metadata_plugin;

    enum class service_state : unsigned char {
        initialized,         ///< the service has initlaized any state required but is idle
        started,             ///< the service is actively running
        stopped              ///< the service is no longer running
    };


    enum class plugin_state : unsigned char {
        registered,  ///< the plugin is constructed but doesn't do anything
        initialized, ///< the plugin has initlaized any state required but is idle
        started,     ///< the plugin is actively running
        stopped      ///< the plugin is no longer running
    };

}
