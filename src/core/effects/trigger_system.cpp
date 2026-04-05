#include "core/effects/trigger_system.h"
#include "core/effects/effect_resolver.h"

namespace core::effects {

void TriggerSystem::QueueActions(const std::vector<Action>& actions) {
    for (const auto& action : actions) {
        EffectResolver::Get().QueueAction(action);
    }
}

}  // namespace core::effects
