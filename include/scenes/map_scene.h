#ifndef DECK_BUILDER_GAME_INCLUDE_SCENES_MAP_SCENE_H_
#define DECK_BUILDER_GAME_INCLUDE_SCENES_MAP_SCENE_H_

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <string>
#include <vector>

#include "core/card_data.h"
#include "engine/scene/scene.h"

namespace scenes {

class MapScene : public engine::Scene {
 public:
  MapScene() : engine::Scene("MapScene") {}
  ~MapScene() override = default;

  void OnAttach() override;
  void OnUpdate(float delta_time_seconds) override;
  void OnRender() override;

 private:
  enum class ScenarioType { Battle, RandomEvent, Shop, Treasure };

  struct ChoiceCard {
    ScenarioType type;
    std::string name;
    glm::vec2 position;
    glm::vec4 color;
    float alpha = 1.0f;
    bool is_hovered = false;
    float shake_timer = 0.0f;
    core::CardData dummy_data;
  };

  void GenerateChoices();
  bool IsMouseOverCard(const ChoiceCard& card, glm::vec2 mouse_pos) const;

  std::vector<ChoiceCard> choices_;
  float fade_timer_ = 0.0f;
  float fade_duration_ = 1.0f;
  bool is_fading_out_ = false;
  bool is_fading_in_ = false;

  const float kTopBarHeight = 64.0f;
  const float kCardScale = 0.8f;
  const float kCardSpacing = 40.0f;
  const float kShakeIntensity = 5.0f;
  const float kShakeSpeed = 20.0f;
};

}  // namespace scenes

#endif  // DECK_BUILDER_GAME_INCLUDE_SCENES_MAP_SCENE_H_
