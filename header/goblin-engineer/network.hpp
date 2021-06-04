#pragma once

#include <boost/asio.hpp>
#include <goblin-engineer/core/abstract_manager_service.hpp>
#include <goblin-engineer/core/forward.hpp>

namespace goblin_engineer { namespace components {

    /// \brief Special manager for network tasks
    /// \details This class inheritance basic behavior from abstract manager and creating specific functionality for networks tasks
    class network_manager_service : public abstract_manager_service {
    public:
        /// \brief Constructor
        /// \details Constructor creating manager service and open <concurency_hint> threads in thread pull
        ///          Each thread run io context
        /// \param[in] env special enviroment for abstract service manager that contains executor for service
        /// \param[in] name name for manager
        /// \param[in] concurrency_hint number of threads
        network_manager_service(
            abstract_manager_service* env, goblin_engineer::string_view name, int concurrency_hint);

        /// \brief Destructor
        /// \details Destructor stopping input-output context
        ~network_manager_service() override;

        /// \brief The function return input-output context of current network manager
        /// \return boost::asio input-output context
        auto loop() -> boost::asio::io_context&;

        /// \brief The function adding message and starting executing him
        /// \details The function is temporarily not thread safe
        /// \param[in] msg message which will be added to manager pull
        /// \param[in] exec_device the parameter is temporarily unused
        void enqueue_base(message_ptr msg, actor_zeta::execution_device* exec_device) override;

    protected:
        boost::asio::io_context io_context_;
        boost::asio::executor_work_guard<boost::asio::io_context::executor_type> work_;
        std::thread thread_;
    };

}} // namespace goblin_engineer::components