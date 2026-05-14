#include "core/card_instance.h"

#include <algorithm>

#include "core/card_data.h"

namespace core {

CardInstance::CardInstance(const CardData* card_data, int inst_id, int owner)
    : data(card_data),
      instance_id(inst_id),
      owner_id(owner),
      location(CardLocation::Deck),
      current_cost(card_data->cost),
      current_power(card_data->power),
      current_health(card_data->health),
      max_health(card_data->health),
      permanent_power_offset(0),
      permanent_health_offset(0),
      has_attacked(false),
      can_attack(false),
      has_haste(false),
      is_blocker(false) {
  // Check for keywords in CardData effects
  for (const auto& effect : data->effects) {
    if (effect.effect_type == "Haste") {
      has_haste = true;
    } else if (effect.effect_type == "Blocker") {
      is_blocker = true;
    }
  }
}

void CardInstance::RecalculateStats() {
  current_power = data->power + permanent_power_offset;
  max_health = data->health + permanent_health_offset;

  for (const auto& mod : modifiers) {
    current_power += mod.power_add;
    max_health += mod.health_add;
  }

  // Ensure health doesn't exceed new max_health, but don't automatically heal
  if (current_health > max_health) {
    current_health = max_health;
  }
}

void CardInstance::ClearModifiers(ModifierDuration duration) {
  modifiers.erase(std::remove_if(modifiers.begin(), modifiers.end(),
                                 [duration](const auto& mod) {
                                   return mod.duration == duration;
                                 }),
                  modifiers.end());
  RecalculateStats();
}

}  // namespace core
