#pragma once

#include <goblin-engineer/forward.hpp>


namespace goblin_engineer {

    class async_policy : public actor_zeta::supervisor {
    public:
        async_policy(actor_zeta::abstract_executor *executor, actor_zeta::detail::string_view view);

        ~async_policy() override = default;

        auto executor() noexcept -> actor_zeta::executor::abstract_executor & override;

        auto join(actor_zeta::actor tmp) -> actor_zeta::actor_address override;

        auto join(actor_zeta::intrusive_ptr<actor_zeta::supervisor> tmp) -> actor_zeta::actor_address;

    private:
        std::unique_ptr<actor_zeta::executor::abstract_executor> coordinator_;
        std::vector<actor_zeta::intrusive_ptr<actor_zeta::supervisor>> storage_;
        std::vector<actor_zeta::actor> actor_storage_;
    };

    class async_policy_lite : public goblin_engineer::supervisor {
    public:
        async_policy_lite(goblin_engineer::abstract_executor *, goblin_engineer::string_view);

        ~async_policy_lite() override = default;

        auto executor() noexcept -> goblin_engineer::abstract_executor & override;

        auto join(goblin_engineer::actor tmp) -> goblin_engineer::actor_address override;

        auto join(goblin_engineer::intrusive_ptr<goblin_engineer::supervisor> tmp) -> goblin_engineer::actor_address;

    private:
        std::unique_ptr<goblin_engineer::abstract_executor> coordinator_;
    };

    class sync_policy : public actor_zeta::supervisor {
    public:
        sync_policy(actor_zeta::supervisor *, actor_zeta::detail::string_view);

        ~sync_policy() override = default;

        auto executor() noexcept -> actor_zeta::abstract_executor & override;

        auto join(actor_zeta::actor) -> actor_zeta::actor_address override;
    };

    template<class BasePolicy>
    struct basic_manager_service_t : public BasePolicy {

        template<class... Args>
        basic_manager_service_t(actor_zeta::supervisor* supervisor, Args &&... args): BasePolicy(std::forward<Args>(args)...) {}

        ~basic_manager_service_t() override = default;
    };

    using abstract_manager_service = basic_manager_service_t<async_policy>;

} // namespace goblin_engineer