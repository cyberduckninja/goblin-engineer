#include <goblin-engineer.hpp>


class logger : public goblin_engineer::abstract_service {

};


class file_storage : public goblin_engineer::abstract_manager_service_heavy {

};


int main() {

    goblin_engineer::dynamic_config cfg;
    goblin_engineer::dynamic_environment app(std::move(cfg));




    app.initialize();
    app.startup();

}