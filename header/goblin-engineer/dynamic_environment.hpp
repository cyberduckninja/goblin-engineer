#pragma once

#include <goblin-engineer/forward.hpp>
#include <goblin-engineer/context.hpp>

namespace goblin_engineer {

    class dynamic_environment final :
            public context_t,
            public abstract_environment {
    public:

        explicit dynamic_environment(dynamic_config&&);

        ~dynamic_environment() override;

        template <typename Manager,typename ...Args>
        auto add_manager_service(Args &&...args) -> Manager* {
            return supervisor<Manager>(config(),env(),std::forward<Args>(args)...);
        }

        void initialize();

        void startup();

        void shutdown();

    private:

        auto env() -> goblin_engineer::abstract_environment *;

        auto config() -> dynamic_config& ;

        auto start() -> std::size_t override ;

        auto executor() -> actor_zeta::executor::abstract_executor & override ;

        auto context() -> context_t *;

        auto main_loop() const ->  boost::asio::io_context& override;

        auto background() const -> boost::thread_group& override;

        goblin_engineer::dynamic_config configuration_;
        std::unique_ptr<actor_zeta::executor::abstract_executor>coordinator_;
        std::unique_ptr<boost::asio::io_context> io_context_;
        std::unique_ptr<boost::thread_group> background_;
    };

}

