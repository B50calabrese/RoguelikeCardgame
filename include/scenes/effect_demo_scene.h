#ifndef DECK_BUILDER_GAME_INCLUDE_SCENES_EFFECT_DEMO_SCENE_H_
#define DECK_BUILDER_GAME_INCLUDE_SCENES_EFFECT_DEMO_SCENE_H_

#include <vector>
#include <memory>
#include <string>

#include "engine/scene/scene.h"
#include "core/state/game_state.h"
#include "core/effects/effect.h"
#include "core/effects/target.h"

namespace scenes {

/**
 * Scene for testing the card effect system.
 */
class EffectDemoScene : public engine::Scene {
 public:
  EffectDemoScene() : Scene("EffectDemoScene") {}

  void OnAttach() override;
  void OnUpdate(float delta_time_seconds) override;
  void OnRender() override;

 private:
  void InitializeGameState();
  void RenderCardInHand(const core::CardInstance& card, float x, float y);
  void RenderCreatureOnBoard(const core::CardInstance& card, float x, float y);
  void RenderPlayerStats();

  void HandleInput();
  void SelectCard(int index);
  void SelectTarget(const core::effects::Target& target);
  void TriggerPendingEffects();

  core::state::GameState state_;

  // Interaction state
  int selected_hand_card_idx_ = -1;
  const core::effects::CardEffectDefinition* pending_effect_def_ = nullptr;
  int pending_source_id_ = -1;
  std::vector<core::effects::Target> selected_targets_;

  std::string status_message_ = "Select a card to play";
};

}  // namespace scenes

#endif  // DECK_BUILDER_GAME_INCLUDE_SCENES_EFFECT_DEMO_SCENE_H_
