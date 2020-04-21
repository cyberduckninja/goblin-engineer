#include <utility>
#include <memory>

#include <goblin-engineer.hpp>
#include <actor-zeta/core.hpp>
#include <iostream>

#include <goblin-engineer/components/http/detail/network.hpp>
#include <goblin-engineer/components/http/http_server/http_session.hpp>
#include <goblin-engineer/components/http/http_server/server.hpp>

namespace goblin_engineer { namespace components { namespace http_server {

server::server(root_manager *env, port_t port)
    : network_manager_service(env, "http", 1) {

  auto http_address = self();

  listener_ = std::make_shared<listener>(
      loop(), port, [=](detail::request_type &&req, std::size_t session_id) {
        detail::query_context context(std::move(req), session_id, http_address);

        actor_zeta::send(
            http_address,
            actor_zeta::make_message(
                http_address
                , "dispatcher"
                , std::move(context)
            )
        );
      }
  );

  add_handler(
      "close",
      &server::close
  );

  add_handler(
      "write",
      &server::write
  );

  add_handler(
      "dispatcher",
      &server::dispatcher
  );

  listener_->run();
}

auto server::add_listener(
    detail::net::io_context &ioc
    , port_t port
    , helper_write_f_t f) {
  std::cerr << "Port = " << port << std::endl;
  listener_ = std::make_shared<listener>(ioc, port, std::move(f));
}

auto server::shutdown() {
  // if (!listener->acceptor_.is_open()) {
  //    return;
  //} else {
  /// listener->acceptor_.
  //}
}

void server::write(detail::response_context_type &body) {
  listener_->write(body);
}

void server::close(detail::response_context_type &) {}

void server::dispatcher() {
  actor_zeta::send(
      addresses("http_dispatcher"),
      std::move(static_cast<actor_zeta::context&>(*this).current_message())
  );
}

}}}