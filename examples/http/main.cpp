#include <goblin-engineer.hpp>
#include <goblin-engineer/components/http.hpp>

using namespace goblin_engineer;
using namespace goblin_engineer::components;

int main() {
    dynamic_config config; ///< Create default config

    root_manager app(config); ///< Create manager with our confing

    http::multiplexer multiplexer_;

    multiplexer_.port(9999);

    http::router router_;

    router_.http_get(
            R"(/my/super/url)",
            [](http::query_context& ctx) {
                ctx.response().body() = ctx.request().body();
                ctx.write();
            }
    );


    auto http1 = app.add_manager_service<http::server>(multiplexer_);
    auto worker = make_service<http::http_dispatcher>(http1,config,router_);
    app.initialize(); /// hook for so/dll
    app.startup();
    return 0;
}