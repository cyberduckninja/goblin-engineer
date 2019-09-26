#include <goblin-engineer.hpp>


class logger : public goblin_engineer::abstract_service {

};


class file_storage final : public goblin_engineer::abstract_manager_service_heavy {
public:
    file_storage(goblin_engineer::dynamic_config&,goblin_engineer::dynamic_environment*env):abstract_manager_service_heavy(env,"file_storage"){}
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

    app.add_manager_service<file_storage>();
    app.add_manager_service<http>();


    app.initialize();
    app.startup();

}