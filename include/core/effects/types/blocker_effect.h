#ifndef DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_TYPES_BLOCKER_EFFECT_H_
#define DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_TYPES_BLOCKER_EFFECT_H_

#include <string>
#include <vector>

#include "core/effects/effect.h"

namespace core::effects::types {

/**
 * @brief Keyword effect: Blocker.
 * If the opposing player has a blocker then the only valid targets are
 * creatures with blocker.
 */
class BlockerEffect : public Effect {
 public:
  std::vector<Action> GenerateActions(
      int source_id, const std::vector<Target>& targets,
      const EffectParams& params) const override {
    // Keywords typically don't generate actions on their own.
    // They are checked by rules or during state changes.
    return {};
  }

  std::string GetDescription(const EffectParams& params) const override {
    return "Blocker";
  }
};

}  // namespace core::effects::types

#endif  // DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_TYPES_BLOCKER_EFFECT_H_
