#pragma once

#include <goblin-engineer/forward.hpp>
#include <goblin-engineer/context.hpp>

namespace goblin_engineer {

    class dynamic_environment final :
            public context_t,
            public actor_zeta::environment::abstract_environment {
    public:

        explicit dynamic_environment(dynamic_config&&);

        ~dynamic_environment() override;

        template <class Manager,typename ...Args>
        auto add_manager_service(Args&&...args){
            return supervisor<Manager>(configuration(), environment(), std::forward<Args>(args)...);
        }

        void initialize();

        void startup();

        void shutdown();

    private:

        auto environment() -> goblin_engineer::dynamic_environment *;

        auto configuration() -> dynamic_config& override ;

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

