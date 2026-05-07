#ifndef DECK_BUILDER_GAME_INCLUDE_SCENES_MAP_SCENE_H_
#define DECK_BUILDER_GAME_INCLUDE_SCENES_MAP_SCENE_H_

#include "engine/scene/scene.h"

namespace scenes {

class MapScene : public engine::Scene {
 public:
  MapScene() : engine::Scene("MapScene") {}
  ~MapScene() override = default;

  void OnAttach() override;
  void OnUpdate(float delta_time_seconds) override;
  void OnRender() override;
};

}  // namespace scenes

#endif  // DECK_BUILDER_GAME_INCLUDE_SCENES_MAP_SCENE_H_
