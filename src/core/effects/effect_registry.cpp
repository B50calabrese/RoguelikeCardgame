#include "core/effects/effect_registry.h"
#include "engine/util/logger.h"

namespace core::effects {

EffectRegistry& EffectRegistry::Get() {
  static EffectRegistry instance;
  return instance;
}

void EffectRegistry::RegisterEffect(const std::string& name, EffectCreator creator) {
  creators_[name] = std::move(creator);
  LOG_INFO("[EffectRegistry] Registered effect: %s", name.c_str());
}

std::unique_ptr<Effect> EffectRegistry::CreateEffect(const std::string& name) const {
  auto it = creators_.find(name);
  if (it != creators_.end()) {
    return it->second();
  }
  return nullptr;
}

}  // namespace core::effects
