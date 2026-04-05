#ifndef DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_VISUAL_BLOCKER_H_
#define DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_VISUAL_BLOCKER_H_

#include <vector>
#include <string>

namespace core::effects {

/**
 * @brief Manages visual "blockers" that prevent the game logic from advancing.
 * This allows animations to play out before the next state change occurs.
 */
class VisualBlocker {
 public:
  static VisualBlocker& Get() {
    static VisualBlocker instance;
    return instance;
  }

  /** @brief Returns true if any visual effects are currently blocking. */
  bool IsBlocking() const {
    return !blockers_.empty();
  }

  /** @brief Adds a named blocker. */
  void AddBlocker(const std::string& name) {
    for (const auto& b : blockers_) {
        if (b == name) return;
    }
    blockers_.push_back(name);
  }

  /** @brief Removes a named blocker. */
  void RemoveBlocker(const std::string& name) {
    for (auto it = blockers_.begin(); it != blockers_.end(); ++it) {
      if (*it == name) {
        blockers_.erase(it);
        return;
      }
    }
  }

  /** @brief Clears all blockers. */
  void Clear() {
    blockers_.clear();
  }

 private:
  VisualBlocker() = default;
  std::vector<std::string> blockers_;
};

}  // namespace core::effects

#endif  // DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_VISUAL_BLOCKER_H_
