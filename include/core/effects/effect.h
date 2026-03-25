#ifndef DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_EFFECT_H_
#define DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_EFFECT_H_

#include <vector>
#include <map>
#include <string>

#include "core/effects/action.h"
#include "core/effects/target_filter.h"
#include "core/effects/trigger.h"

namespace core::effects {

/**
 * @brief Parameters for an effect instance.
 */
using EffectParams = std::map<std::string, std::string>;

/**
 * @brief Interface for a card effect.
 */
class Effect {
 public:
  virtual ~Effect() = default;

  /** @brief Produces actions when triggered. */
  virtual std::vector<Action> GenerateActions(int source_id, const std::vector<Target>& targets, const EffectParams& params) const = 0;

  virtual const TargetFilter& GetTargetFilter() const { return default_filter_; }

 private:
  TargetFilter default_filter_;
};

/**
 * @brief Definition of an effect on a card.
 */
struct CardEffectDefinition {
  Trigger trigger;
  std::string effect_type;  // Name of the effect in the registry
  EffectParams params;
  TargetFilter filter;
};

}  // namespace core::effects

namespace core {
  using effects::Trigger;
  using effects::CardEffectDefinition;
}

#endif  // DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_EFFECT_H_
