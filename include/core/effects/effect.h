#ifndef DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_EFFECT_H_
#define DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_EFFECT_H_

#include <vector>
#include <map>
#include <string>

#include "core/effects/action.h"
#include "core/effects/target_filter.h"

namespace core {

/**
 * @brief Common triggers for card effects.
 */
enum class Trigger {
  OnPlay,
  OnDeath,
  AtStartOfTurn,
  AtEndOfTurn,
  Manual  // For effects that don't trigger automatically
};

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

  /**
   * @brief Produces actions when this effect is triggered.
   * @param source_id The ID of the card or source of the effect.
   * @param target_id The target ID (if already selected, else can be 0).
   * @param params Parameters loaded for this specific card effect instance.
   * @return A list of actions that should be queued.
   */
  virtual std::vector<Action> GenerateActions(int source_id, const std::vector<Target>& targets, const EffectParams& params) const = 0;

  /**
   * @brief Returns the targeting requirements for this effect.
   */
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

}  // namespace core

#endif  // DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_EFFECT_H_
