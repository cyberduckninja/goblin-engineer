#include <iostream>

#include <goblin-engineer.hpp>
#include <goblin-engineer/components/http.hpp>

using namespace goblin_engineer;
using namespace goblin_engineer::components;

int main() {
  root_manager app(1, 1000); ///< Create manager with our confing

  http::multiplexer multiplexer_;

  http::router router_;

  router_.http_get(
      R"(/my/super/url)"
      , [](http::query_context &ctx) {
          std::cerr << ctx.response().body().c_str() << std::endl;
          ctx.response().body() = ctx.request().body();
          ctx.write();
        }
  );

  auto http = make_manager_service<http::server>(app, 9999);
  make_service<http::http_dispatcher>(http, router_);

  auto sigint_set = std::make_shared<boost::asio::signal_set>(app.loop(), SIGINT, SIGTERM);
  sigint_set->async_wait(
      [sigint_set](const boost::system::error_code & /*err*/,int /*num*/) {
        sigint_set->cancel();
      }
  );

  app.startup();
  return 0;
}