#pragma once

#include <goblin-engineer/forward.hpp>
#include <goblin-engineer/abstract_manager_service.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>

namespace goblin_engineer { namespace components {

    /// \brief Special manager for base tasks
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
                root_manager *env
                , actor_zeta::detail::string_view name
                , std::size_t concurrency_hint
        );
        
        /// \brief Destructor
        /// \details Destructor stopping input-output context
        ~network_manager_service() override;
        
        /// \brief The function return input-output context of current base manager
        /// \return boost::asio input-output context  
        auto loop() -> boost::asio::io_context &;
        
        /// \brief The function adding message and starting executing him
        /// \details The function is temporarily not thread safe
        /// \param[in] msg message which will be added to manager pull
        /// \param[in] exec_device the parameter is temporarily unused
        void enqueue(message msg, actor_zeta::executor::execution_device *exec_device) override;

    protected:
        boost::asio::io_context io_context_;                                                
        boost::asio::executor_work_guard<boost::asio::io_context::executor_type> work_;     
        boost::thread_group &threadpool_;                                                   
    };

}}