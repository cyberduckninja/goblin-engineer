#include <goblin-engineer.hpp>

int main() {
    goblin_engineer::dynamic_config cfg;
    goblin_engineer::root_manager app(std::move(cfg));

    app.initialize();
    app.startup();

    return 0;
}