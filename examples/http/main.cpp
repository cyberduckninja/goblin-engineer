#include <goblin-engineer.hpp>
#include <goblin-engineer/components/http.hpp>


using namespace goblin_engineer;

int main() {
    dynamic_config config; ///< Create default config
    root_manager app(config); ///< Create manager with our confing
    multiplexer
            .port(9999)
            .address_default(); /// -> .address()

    router.http_get(
            R"(/my/super/url)",
            [](auto context& ctx) {
        return ctx->response().set_body(ctx.request().body()).done();
    });

    configuration cfg(multiplexer,router)

    auto http1 = app.add_manager_service<http_t>(сfg_router);
    auto worker = make_service<worker_t>(http1,std::move(сfg_router));
    app.initialize(); // hook for so/dll
    app.startup();
    return 0;
}