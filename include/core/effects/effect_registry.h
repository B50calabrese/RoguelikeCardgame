#ifndef DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_EFFECT_REGISTRY_H_
#define DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_EFFECT_REGISTRY_H_

#include <memory>
#include <string>
#include <unordered_map>
#include <functional>

#include "core/effects/effect.h"

namespace core {

/**
 * @brief Singleton registry that manages all available effect types.
 */
class EffectRegistry {
 public:
  using EffectCreator = std::function<std::unique_ptr<Effect>()>;

  static EffectRegistry& Get();

  /**
   * @brief Registers a new effect type.
   */
  void RegisterEffect(const std::string& name, EffectCreator creator);

  /**
   * @brief Creates an instance of an effect by name.
   */
  std::unique_ptr<Effect> CreateEffect(const std::string& name) const;

 private:
  EffectRegistry() = default;
  ~EffectRegistry() = default;
  EffectRegistry(const EffectRegistry&) = delete;
  EffectRegistry& operator=(const EffectRegistry&) = delete;

  std::unordered_map<std::string, EffectCreator> creators_;
};

}  // namespace core

#endif  // DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_EFFECT_REGISTRY_H_
