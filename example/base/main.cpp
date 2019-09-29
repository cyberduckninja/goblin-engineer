#include <goblin-engineer.hpp>

class http;
class file_storage;

class logger : public goblin_engineer::abstract_service {
public:
    explicit logger(http *manager):goblin_engineer::abstract_service(manager,"logger"){}
    ~logger() override = default;
};


class file_storage final : public goblin_engineer::abstract_manager_service {
public:
    file_storage(goblin_engineer::dynamic_config&,goblin_engineer::dynamic_environment*env):abstract_manager_service(env,"file_storage"){}
    ~file_storage() override = default;
};

class http final : public goblin_engineer::abstract_manager_service {
public:
    http(goblin_engineer::dynamic_config&,goblin_engineer::dynamic_environment*env):abstract_manager_service(env,"http"){}
    ~http() override = default;
};



int main() {

    goblin_engineer::dynamic_config cfg;
    goblin_engineer::dynamic_environment app(std::move(cfg));

    auto* files = app.add_manager_service<file_storage>();
    auto* http1 = app.add_manager_service<http>();

    auto log =  goblin_engineer::make_service<logger>(http1);


    app.initialize();
    app.startup();
    return 0;
}