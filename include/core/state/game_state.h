#ifndef DECK_BUILDER_GAME_INCLUDE_CORE_STATE_GAME_STATE_H_
#define DECK_BUILDER_GAME_INCLUDE_CORE_STATE_GAME_STATE_H_

#include <vector>
#include "core/enums.h"

namespace core {

/**
 * @brief Singleton class that tracks the overall state of a game run.
 */
class GameState {
 public:
  static GameState& Get();

  void Reset();

  // Getters and Setters
  CharacterType character_type() const { return character_type_; }
  void set_character_type(CharacterType type) { character_type_ = type; }

  const std::vector<CardColor>& colors() const { return colors_; }
  void set_colors(const std::vector<CardColor>& colors) { colors_ = colors; }

  const std::vector<int>& deck() const { return deck_; }
  void set_deck(const std::vector<int>& deck) { deck_ = deck; }
  void add_card_to_deck(int card_id) { deck_.push_back(card_id); }

 private:
  GameState();
  ~GameState() = default;
  GameState(const GameState&) = delete;
  GameState& operator=(const GameState&) = delete;

  CharacterType character_type_ = CharacterType::None;
  std::vector<CardColor> colors_;
  std::vector<int> deck_;
};

}  // namespace core

#endif  // DECK_BUILDER_GAME_INCLUDE_CORE_STATE_GAME_STATE_H_
