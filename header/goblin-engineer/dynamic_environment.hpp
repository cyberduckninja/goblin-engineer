#pragma once

#include <boost/asio.hpp>
#include <boost/thread.hpp>

#include <goblin-engineer/forward.hpp>
#include <goblin-engineer/dynamic.hpp>
#include <actor-zeta/core.hpp>

namespace goblin_engineer {

    class dynamic_environment final {
    public:

        explicit dynamic_environment(dynamic_config&&);

        ~dynamic_environment();

        template <class Manager,typename ...Args>
        auto add_manager_service(Args&&...args)-> Manager* {
            auto * tmp  = new Manager(configuration(), environment(), std::forward<Args>(args)...);
            storage_.emplace_back(actor_zeta::intrusive_ptr<Manager>(tmp));
            return tmp;
        }

        void initialize();

        void startup();

        void shutdown();

        auto executor() -> actor_zeta::executor::abstract_executor & ;

        auto background() const -> boost::thread_group &;

    private:

        auto environment() -> goblin_engineer::dynamic_environment *;

        auto configuration() -> dynamic_config&;

        auto start() -> std::size_t ;

        goblin_engineer::dynamic_config configuration_;
        std::unique_ptr<actor_zeta::executor::abstract_executor>coordinator_;
        std::unique_ptr<boost::asio::io_context> io_context_;
        std::unique_ptr<boost::thread_group> background_;
        std::vector<actor_zeta::intrusive_ptr<actor_zeta::supervisor>> storage_;
    };

}

