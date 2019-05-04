#include <goblin-engineer/dynamic_environment.hpp>

#include <forward_list>
#include <iostream>

#include <actor-zeta/environment/cooperation.hpp>
#include <actor-zeta/executor/coordinator.hpp>
#include <actor-zeta/executor/policy/work_sharing.hpp>

#include <goblin-engineer/dynamic.hpp>
#include <goblin-engineer/metadata.hpp>
#include <goblin-engineer/abstract_service.hpp>
#include <goblin-engineer/plugin.hpp>
#include <goblin-engineer/data_provider.hpp>


namespace goblin_engineer {

    inline std::string name_plugin(abstract_plugin *ptr) {
        std::string tmp;
        std::unique_ptr<metadata_plugin> metadata(new metadata_plugin);
        ptr->metadata(metadata.get());
        tmp = metadata->name;
        return tmp;
    }

    void startup(data_provider&provider,context_t *context) {
        std::cerr << "startup service: " << provider.name() << std::endl;
        ///if (state() == service_state::initialized) {
        ///    state(service_state::started);
        /**    return*/ provider.startup(context);
        ///} else {
        ///    std::cerr << "error startup  service: " << provider.name() << std::endl;
        ///}
    }

    void shutdown(data_provider&provider ) {
        std::cerr << "shutdown service:" << provider.name() << std::endl;
        ///if (state() == service_state::started) {
        ///    state(service_state::stopped);
        /**    return*/ provider.shutdown();
        ///} else {
        ///    std::cerr << "error shutdown service:" << provider.name() << std::endl;
        ///}

    }

    struct dynamic_environment::impl final {
        impl() :
                coordinator_(new actor_zeta::executor::coordinator<actor_zeta::executor::work_sharing>(1, 1000)),
                io_serv(new boost::asio::io_service),
                background_(new boost::thread_group) {
        }

        ~impl() = default;

        auto add_plugin(abstract_plugin *plugin_ptr) -> void {
            auto size = storage_plugin.size();
            storage_plugin.emplace_back(plugin_ptr);
            mapper.emplace(name_plugin(plugin_ptr), size);
            state_plugin.emplace_back(size);
        }

        auto get_plugin(std::size_t index) -> plugin & {
            return storage_plugin.at(index);
        }

        auto get_plugin(const std::string &__name__) -> plugin & {
            auto index = mapper.find(__name__);
            return storage_plugin.at(index->second);
        }

        plugin_state state(std::size_t index) const {
            return storage_plugin.at(index).state();
        }

        std::vector<std::size_t> &current_state() {
            return state_plugin;
        }

        auto main_loop() -> boost::asio::io_service * {
            return io_serv.get();
        }

        auto background() const -> boost::thread_group & {
            return *background_;
        }

        auto configuration() -> dynamic_config & {
            return configuration_.dynamic_configuration;
        }

        ///Config
        goblin_engineer::configuration configuration_;
        ///Config

        actor_zeta::environment::cooperation cooperation_;
        std::unique_ptr<actor_zeta::executor::abstract_coordinator>coordinator_;

        std::unordered_map<std::string,std::unique_ptr<data_provider> >  data_provider_;

    private:

        /// plugin
        std::vector<plugin> storage_plugin;
        std::unordered_map<std::string, std::size_t> mapper;
        std::vector<std::size_t> state_plugin;
        /// plugin

        std::unique_ptr<boost::asio::io_service> io_serv;
        std::unique_ptr<boost::thread_group> background_;

    };

    void dynamic_environment::shutdown() {

        for (auto &i:pimpl->data_provider_) {
            goblin_engineer::shutdown((*i.second.get()));
        }

        /// TODO: Plugin
        /*
        for (const auto &i:pimpl->current_state()) {
            if (pimpl->state(i) == plugin_state::started) {
                pimpl->get_plugin(i).shutdown();
            }
        }
        */
        pimpl->main_loop()->stop();

    }

    void dynamic_environment::startup() {

        for (auto &i:pimpl->data_provider_) {
            goblin_engineer::startup(*(i.second.get()),static_cast<context_t*>(this));
        }

        /// TODO: Plugin
        /*
        for (const auto &i:pimpl->current_state()) {
            if (pimpl->state(i) == plugin_state::initialized) {
                pimpl->get_plugin(i).startup(static_cast<context_t*>(this));
            }
        }
        */

        start();

        shutdown();
    }

    void dynamic_environment::initialize() {
        /// TODO: Plugin
        /*
        for (const auto &i:pimpl->configuration_.plugins) {
            auto &plugin = pimpl->get_plugin(i);
            if (plugin.state() == plugin_state::registered) {
                plugin.initialization();
            }
        }
         */

    }

    dynamic_environment::dynamic_environment(configuration &&f) : pimpl(new impl) {
        pimpl->configuration_ = std::move(f);

        std::shared_ptr<boost::asio::signal_set> sigint_set(new boost::asio::signal_set(main_loop(), SIGINT, SIGTERM));
        sigint_set->async_wait(
                [sigint_set, this](const boost::system::error_code &/*err*/, int /*num*/) {
                    shutdown();
                    sigint_set->cancel();
                }
        );

    }


    dynamic_environment::~dynamic_environment() {
        pimpl->background().join_all();
        pimpl->main_loop()->stopped();
        std::cerr << "~goblin-engineer" << std::endl;
    }

    auto dynamic_environment::context() -> context_t * {
        return static_cast<context_t *>(this);
    }

    boost::asio::io_service &dynamic_environment::main_loop() const {
        return *pimpl->main_loop();
    }

    boost::thread_group &dynamic_environment::background() const {
        return pimpl->background();
    }

    int dynamic_environment::start() {
        manager_execution_device().start();
        return pimpl->main_loop()->run();
    }

    auto dynamic_environment::manager_execution_device() -> actor_zeta::executor::abstract_coordinator & {
        return *pimpl->coordinator_;
    }

    auto dynamic_environment::manager_group() -> actor_zeta::environment::cooperation & {
        return pimpl->cooperation_;
    }

    auto dynamic_environment::add_plugin(abstract_plugin *plugin_ptr) -> void {
        return pimpl->add_plugin(plugin_ptr);
    }

    auto dynamic_environment::add_service(abstract_service *service_ptr) -> service & {
        service_ptr->startup(context());
        return manager_group().created_group(service_ptr);
    }

    auto dynamic_environment::config() const -> dynamic_config & {
        return pimpl->configuration();
    }

    auto dynamic_environment::env() -> goblin_engineer::abstract_environment  * {
        return static_cast<goblin_engineer::abstract_environment  *>(this);
    }

    auto dynamic_environment::add_data_provider(data_provider *ptr ) -> data_provider & {
       auto name_  =  ptr->name();
       pimpl->data_provider_.emplace(name_,std::unique_ptr<data_provider>(ptr));
       return *(pimpl->data_provider_.at(name_).get());
    }

}