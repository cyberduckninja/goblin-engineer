#include <goblin-engineer/data_provider.hpp>
#include <goblin-engineer/context.hpp>
#include <cassert>

namespace goblin_engineer {

    bool data_provider::send(message &&message) {
        {
            actor_zeta::behavior::context context_(this, std::move(message));
            reactions_.execute(context_); /** context processing */
        }
        return true;

    }

    data_provider::data_provider(goblin_engineer::context_t *context, const std::string &name):sync_actor(context->env(),name){

    }

    bool data_provider::send(actor_zeta::messaging::message && message, actor_zeta::executor::execution_device *) {
        {
            actor_zeta::behavior::context context_(this, std::move(message));
            reactions_.execute(context_); /** context processing */
        }
    }

    void data_provider::launch(actor_zeta::executor::execution_device *, bool) {

    }

    data_provider::~data_provider()  = default;

}

