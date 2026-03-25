---
name: create-scene
description: Instructions for creating and registering a new game scene.
---

# Creating a New Scene

Use this skill when you need to add a new screen or state to the game (e.g., Shop, Deck Editor, Battle Result).

## Steps to Create a Scene

### 1. Inherit from `engine::Scene`
Create a new header file in `include/scenes/` (e.g., `my_scene.h`) and a corresponding source file in `src/scenes/` (e.g., `my_scene.cpp`).

#### Header Template:
```cpp
#ifndef DECK_BUILDER_GAME_INCLUDE_SCENES_MY_SCENE_H_
#define DECK_BUILDER_GAME_INCLUDE_SCENES_MY_SCENE_H_

#include "engine/scene/scene.h"

namespace scenes {

class MyScene : public engine::Scene {
 public:
  MyScene() : engine::Scene("MyScene") {}

  void OnAttach() override;   // Called when the scene is loaded
  void OnUpdate(float delta_time_seconds) override; // Logic update
  void OnRender() override;   // Rendering logic
  bool OnInput() override;    // Input handling (returns true if handled)

 private:
  // Your private members and helper functions
};

}  // namespace scenes

#endif
```

### 2. Implement Lifecycle Methods
In `src/scenes/my_scene.cpp`, implement the required methods. Use `OnAttach` to initialize textures, UI components, and load data.

### 3. Add to Build System
Update `CMakeLists.txt` to include the new source file in the `SOURCE_FILES` list.

### 4. Transition to the New Scene
Use the `engine::SceneManager` to switch scenes. Typically this is done in response to a button click or game event:

```cpp
#include "engine/scene/scene_manager.h"
#include "scenes/my_scene.h"

// Inside a handler in another scene:
engine::SceneManager::Get().SetScene(std::make_unique<scenes::MyScene>());
```

## Verification
1. Ensure the new `.h` and `.cpp` files follow the naming and directory conventions.
2. Confirm the scene is added to `CMakeLists.txt`.
3. Verify that the transition logic (if any) correctly points to the new scene class.

## Code Reference
- Base Class: `GameEngine/include/engine/scene/scene.h`
- Scene Manager: `GameEngine/include/engine/scene/scene_manager.h`
- Existing Scenes: `include/scenes/`
