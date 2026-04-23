#ifndef DECK_BUILDER_GAME_INCLUDE_SCENES_COMBAT_COMBAT_STATE_H_
#define DECK_BUILDER_GAME_INCLUDE_SCENES_COMBAT_COMBAT_STATE_H_

namespace scenes {

enum class CombatState {
    Idle,
    PickingTarget,
    AnimatingAttack,
    PickingEffectTarget
};

} // namespace scenes

#endif // DECK_BUILDER_GAME_INCLUDE_SCENES_COMBAT_COMBAT_STATE_H_
