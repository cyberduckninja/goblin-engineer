#include <goblin-engineer.hpp>
#include <goblin-engineer/components/http.hpp>

using namespace goblin_engineer;
using namespace goblin_engineer::components;

int main() {
    dynamic_config config; ///< Create default config
    root_manager app(config); ///< Create manager with our confing

    http::multiplexer multiplexer_;

    multiplexer_.port(9999)
                .address_default_v4();

    http::router router_;

    router_.http_get(
            R"(/my/super/url)",
            [](auto context& ctx) {
                return ctx->response().set_body(ctx.request().body()).done();
            }
    );

    configuration cfg(multiplexer,router);

    auto http1 = app.add_manager_service<http::server>(сfg_router);
    auto worker = make_service<http::http_dispatcher>(http1,std::move(сfg_router));
    app.initialize(); /// hook for so/dll
    app.startup();
    return 0;
}