#pragma once

#include <goblin-engineer/core/forward.hpp>


namespace goblin_engineer {

    class async_policy : public actor_zeta::supervisor_t {
    public:

        async_policy(goblin_engineer::intrusive_ptr<goblin_engineer::supervisor_t>, goblin_engineer::string_view view);

        async_policy(goblin_engineer::string_view view);

        ~async_policy() override = default;

        auto join(goblin_engineer::actor ) -> goblin_engineer::actor_address override;

        auto join(goblin_engineer::intrusive_ptr<goblin_engineer::supervisor_t> ) -> goblin_engineer::actor_address;

    protected:
        virtual auto  get_executor() noexcept -> goblin_engineer::abstract_executor* = 0;
        std::vector<actor_zeta::intrusive_ptr<actor_zeta::supervisor_t>> storage_;
        std::vector<actor_zeta::actor> actor_storage_;
    };

    class async_policy_lite : public goblin_engineer::supervisor_t {
    public:
        async_policy_lite(goblin_engineer::intrusive_ptr<goblin_engineer::supervisor_t>, goblin_engineer::string_view view);

        async_policy_lite(goblin_engineer::string_view view);

        ~async_policy_lite() override = default;

        auto join(goblin_engineer::actor ) -> goblin_engineer::actor_address override;

        auto join(goblin_engineer::intrusive_ptr<goblin_engineer::supervisor_t> ) -> goblin_engineer::actor_address;

    protected:
        virtual auto  get_executor() noexcept -> goblin_engineer::abstract_executor* = 0;

    };

    class sync_policy : public actor_zeta::supervisor_t {
    public:
        sync_policy(goblin_engineer::intrusive_ptr<goblin_engineer::supervisor_t>, goblin_engineer::string_view);

        sync_policy(goblin_engineer::string_view view);

        ~sync_policy() override = default;

        auto executor() noexcept -> goblin_engineer::abstract_executor* override;

        auto join(goblin_engineer::actor) -> goblin_engineer::actor_address override;
    };

    template<class BasePolicy>
    struct basic_manager_service_t : public BasePolicy {

        template<class... Args>
        basic_manager_service_t(Args &&... args): BasePolicy(std::forward<Args>(args)...) {}

        ~basic_manager_service_t() override = default;
    };

    using abstract_manager_service = basic_manager_service_t<async_policy>;

} // namespace goblin_engineer