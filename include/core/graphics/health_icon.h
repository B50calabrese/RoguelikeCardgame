#ifndef DECK_BUILDER_GAME_INCLUDE_CORE_GRAPHICS_HEALTH_ICON_H_
#define DECK_BUILDER_GAME_INCLUDE_CORE_GRAPHICS_HEALTH_ICON_H_

#include <string>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

namespace core::graphics {

/**
 * @brief Renders a health icon for a player or enemy.
 * Currently renders as a colored square with health text.
 * Designed to be replaceable with a texture in the future.
 */
class HealthIcon {
 public:
  HealthIcon(const glm::vec2& position, float size, const glm::vec4& color);

  /**
   * @brief Renders the health icon.
   * @param health The current health value to display.
   */
  void Render(int health) const;

  void set_position(const glm::vec2& position) { position_ = position; }
  void set_size(float size) { size_ = size; }
  void set_color(const glm::vec4& color) { color_ = color; }

  const glm::vec2& position() const { return position_; }
  float size() const { return size_; }

 private:
  glm::vec2 position_;
  float size_;
  glm::vec4 color_;
  std::string texture_path_; // Future use
};

}  // namespace core::graphics

#endif  // DECK_BUILDER_GAME_INCLUDE_CORE_GRAPHICS_HEALTH_ICON_H_
