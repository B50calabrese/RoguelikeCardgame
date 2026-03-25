#ifndef DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_ACTION_H_
#define DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_ACTION_H_

#include <memory>
#include "core/effects/actions/action_base.h"

namespace core::effects {
  using Action = std::shared_ptr<actions::ActionBase>;
}

#endif  // DECK_BUILDER_GAME_INCLUDE_CORE_EFFECTS_ACTION_H_
