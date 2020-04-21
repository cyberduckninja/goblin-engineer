#pragma once

#include <goblin-engineer.hpp>
#include <goblin-engineer/components/root_manager.hpp>
#include <goblin-engineer/components/network.hpp>
#include <goblin-engineer/components/http/http_server/listener.hpp>

namespace goblin_engineer { namespace components { namespace http_server {

class server final : public network_manager_service {
public:
  server(root_manager *, port_t);

  ~server() override = default;

private:
  void write(detail::response_context_type &body);

  void close(detail::response_context_type &body);

  auto add_listener(detail::net::io_context &, port_t, helper_write_f_t);

  auto shutdown();

  void dispatcher();

  std::shared_ptr<listener> listener_;
};

}}}