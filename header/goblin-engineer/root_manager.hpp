#pragma once

#include <boost/asio.hpp>
#include <boost/thread.hpp>

#include <goblin-engineer/forward.hpp>
#include <goblin-engineer/dynamic.hpp>
#include <actor-zeta/core.hpp>

namespace goblin_engineer {

    class root_manager final : public  actor_zeta::supervisor {
    public:

        explicit root_manager(dynamic_config);

        ~root_manager() override;

        template <class Manager,typename ...Args>
        auto add_manager_service(Args&&...args) {
            actor_zeta::intrusive_ptr<Manager> tmp (
                    new Manager(
                            environment(),
                            configuration(),
                            std::forward<Args>(args)...
                    )
            );
            join(tmp);
            return tmp;
        }

        void initialize();

        void startup();

        void shutdown();

        auto executor() noexcept -> actor_zeta::executor::abstract_executor & override ;

        auto background() const -> boost::thread_group &;

    private:

        auto broadcast(message) -> bool override;

         auto join(base_actor * ) -> actor_zeta::actor::actor_address override;

         auto join(actor_zeta::intrusive_ptr<actor_zeta::supervisor> tmp) -> actor_zeta::actor::actor_address;

        void enqueue(message, actor_zeta::executor::execution_device *) override;

        auto environment() -> root_manager *;

        auto configuration() -> dynamic_config&;

        auto start() -> std::size_t ;

        goblin_engineer::dynamic_config configuration_;
        std::unique_ptr<actor_zeta::executor::abstract_executor>coordinator_;
        std::unique_ptr<boost::asio::io_context> io_context_;
        std::unique_ptr<boost::thread_group> background_;
        std::vector<actor_zeta::intrusive_ptr<actor_zeta::supervisor>> storage_;
    };

}

