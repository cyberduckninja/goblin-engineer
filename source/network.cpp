#include <goblin-engineer/components/root_manager.hpp>
#include <actor-zeta/core.hpp>
#include <goblin-engineer/components/network.hpp>

namespace goblin_engineer { namespace components {

        network_manager_service::network_manager_service(
                root_manager *env,
                actor_zeta::detail::string_view name,
                std::size_t concurrency_hint
        )
        : abstract_manager_service(env, name)
        , io_context_(concurrency_hint)
        , work_(boost::asio::make_work_guard(io_context_)) {
          thread_ = std::thread([this](){
            io_context_.run();
          });
        }

        network_manager_service::~network_manager_service() {
            io_context_.stop();
            thread_.join();
        }

        auto network_manager_service::loop() -> boost::asio::io_context & {
            return io_context_;
        }

        ///TODO: problem in non thread safe
        void network_manager_service::enqueue(message msg, actor_zeta::executor::execution_device *) {
          set_current_message(std::move(msg));
          dispatch().execute(*this);
        }

}}