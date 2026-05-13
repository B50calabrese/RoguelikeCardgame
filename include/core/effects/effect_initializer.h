#ifndef DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_EFFECT_INITIALIZER_H_
#define DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_EFFECT_INITIALIZER_H_

namespace core::effects {

/**
 * @brief Utility class to initialize and register all game effects.
 */
class EffectInitializer {
 public:
  /** @brief Registers all standard game effects to the EffectRegistry. */
  static void InitializeAll();
};

}  // namespace core::effects

#endif  // DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_EFFECT_INITIALIZER_H_
