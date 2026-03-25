---
name: add-ui-component
description: Instructions for using common UI components like buttons.
---

# Adding UI Components

Use this skill when you need to add interactive UI elements (like buttons, sliders, or text labels) to a scene.

## Common UI Components

### 1. UIButton (`core::graphics::UIButton`)
A simple, reusable button with a label and a click callback.

#### How to Add a Button:
In your Scene class (`include/scenes/my_scene.h`):
```cpp
#include "core/graphics/ui_button.h"
#include <vector>
#include <memory>

// In MyScene class:
private:
  std::vector<std::unique_ptr<core::graphics::UIButton>> buttons_;
```

In `src/scenes/my_scene.cpp`:
```cpp
void MyScene::OnAttach() {
  buttons_.push_back(std::make_unique<core::graphics::UIButton>(
      "Play Game",            // Label
      glm::vec2{400, 300},    // Position (pixel coords)
      glm::vec2{200, 50},     // Size
      []() {                  // Callback function
          std::cout << "Button clicked!" << std::endl;
      }
  ));
}

void MyScene::OnUpdate(float delta_time) {
  // Use InputManager to get mouse state
  glm::vec2 mouse_pos = engine::InputManager::Get().GetMousePosition();
  bool clicked = engine::InputManager::Get().IsMouseButtonPressed(engine::MOUSE_BUTTON_LEFT);

  for (auto& button : buttons_) {
    button->Update(mouse_pos, clicked);
  }
}

void MyScene::OnRender() {
  for (const auto& button : buttons_) {
    button->Render();
  }
}
```

### 2. Text Rendering
Use `engine::graphics::TextRenderer` for general text display.

```cpp
#include "engine/graphics/text_renderer.h"

// In OnRender:
engine::graphics::TextRenderer::Get().RenderText(
    "Mage Tower",           // Text
    glm::vec2{100, 100},    // Position
    1.0f,                   // Scale
    glm::vec3{1, 1, 1}      // Color
);
```

## Best Practices
- **Scaling**: Position UI elements relative to the window size if possible (`core::GameConfig::Get().window_width`).
- **Input**: Only update UI components if they are on the active scene and not covered by another UI layer.
- **Verification**: Always test button callbacks to ensure they execute correctly (e.g., scene transition, logging).

## Code Reference
- UIButton Header: `include/core/graphics/ui_button.h`
- Input Manager: `GameEngine/include/engine/input/input_manager.h`
- Text Renderer: `GameEngine/include/engine/graphics/text_renderer.h`
