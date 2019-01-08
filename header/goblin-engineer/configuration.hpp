#pragma once

#include <unordered_set>

#include <boost/filesystem.hpp>
#include <goblin-engineer/dynamic.hpp>

namespace goblin_engineer {

    struct configuration final {
        dynamic_config                  dynamic_configuration;
        boost::filesystem::path         plugins_dir;
        boost::filesystem::path         data_dir;
        std::unordered_set<std::string> plugins;
    };
}