#include <goblin-engineer/data_provider.hpp>
#include <goblin-engineer/context.hpp>
#include <cassert>

namespace goblin_engineer {

    bool data_provider::send(message message) {
        {
            actor_zeta::actor::context context_(this, std::move(message));
            dispatch().execute(context_); /** context processing */
        }
        return true;

    }

    data_provider::data_provider(actor_zeta::environment::abstract_environment *env,  const std::string &name):actor_zeta::actor::local_actor(env,name){

    }

    bool data_provider::send(actor_zeta::messaging::message  message, actor_zeta::executor::execution_device *) {
        {
            actor_zeta::actor::context context_(this, std::move(message));
            dispatch().execute(context_); /** context processing */
        }
        return true;
    }

    void data_provider::launch(actor_zeta::executor::execution_device *, bool) {

    }


}

