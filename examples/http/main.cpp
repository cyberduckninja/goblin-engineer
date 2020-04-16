#include <iostream>

#include <goblin-engineer.hpp>
#include <goblin-engineer/components/http.hpp>

using namespace goblin_engineer;
using namespace goblin_engineer::components;

int main() {
    dynamic_config config; ///< Create default config

    root_manager app(1,1000); ///< Create manager with our confing

    http::multiplexer multiplexer_;

    http::router router_;

    router_.http_get(
            R"(/my/super/url)",
            [](http::query_context& ctx) {
                std::cerr << ctx.response().body().c_str() << std::endl;
                ctx.response().body() = ctx.request().body();
                ctx.write();
            }
    );


    auto http1 =  make_manager_service<http::server>(app,config,9999);
    auto worker = make_service<http::http_dispatcher>(http1,config,router_);
    app.startup();
    return 0;
}