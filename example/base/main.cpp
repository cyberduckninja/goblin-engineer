#include <goblin-engineer.hpp>


class logger : public goblin_engineer::abstract_service {
public:
    explicit logger(goblin_engineer::abstract_manager_service_lite*manager):abstract_service(manager,"logger"){}
    ~logger() override = default;
};


class file_storage final : public goblin_engineer::abstract_manager_service_lite {
public:
    file_storage(goblin_engineer::dynamic_config&,goblin_engineer::dynamic_environment*env):abstract_manager_service_lite(env,"file_storage"){}
    ~file_storage() override = default;
    void startup(goblin_engineer::context_t *) override {}
};

class http final : public goblin_engineer::abstract_manager_service_lite {
public:
    http(goblin_engineer::dynamic_config&,goblin_engineer::dynamic_environment*env):abstract_manager_service_lite(env,"file_storage"){}
    ~http() override = default;
    void startup(goblin_engineer::context_t *) override {}
};



int main() {

    goblin_engineer::dynamic_config cfg;
    goblin_engineer::dynamic_environment app(std::move(cfg));

    auto* files = app.add_manager_service<file_storage>();
    auto* http1 = app.add_manager_service<http>();

///    http1->join<logger>(http1);


    app.initialize();
    app.startup();

}