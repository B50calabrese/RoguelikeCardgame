#include "core/effects/effect_initializer.h"

#include <memory>

#include "core/effects/effect_registry.h"
#include "core/effects/types/blocker_effect.h"
#include "core/effects/types/damage_effect.h"
#include "core/effects/types/draw_effect.h"
#include "core/effects/types/haste_effect.h"
#include "core/effects/types/heal_effect.h"
#include "core/effects/types/modify_max_mana_effect.h"
#include "core/effects/types/stat_modify_effect.h"

namespace core::effects {

void EffectInitializer::InitializeAll() {
  auto& registry = EffectRegistry::Get();

  registry.RegisterEffect("Damage", []() {
    return std::make_unique<core::effects::types::DamageEffect>();
  });
  registry.RegisterEffect("Draw", []() {
    return std::make_unique<core::effects::types::DrawEffect>();
  });
  registry.RegisterEffect("Buff", []() {
    return std::make_unique<core::effects::types::StatModifyEffect>();
  });
  registry.RegisterEffect("Heal", []() {
    return std::make_unique<core::effects::types::HealEffect>();
  });
  registry.RegisterEffect("ModifyMaxMana", []() {
    return std::make_unique<core::effects::types::ModifyMaxManaEffect>();
  });
  registry.RegisterEffect("Haste", []() {
    return std::make_unique<core::effects::types::HasteEffect>();
  });
  registry.RegisterEffect("Blocker", []() {
    return std::make_unique<core::effects::types::BlockerEffect>();
  });
}

}  // namespace core::effects
