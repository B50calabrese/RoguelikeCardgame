#include "core/effects/rules_engine.h"
#include "core/effects/rules/turn_rule.h"
#include "core/effects/rules/mana_rule.h"
#include "core/effects/rules/targeting_rule.h"

namespace core::effects {

RulesEngine::RulesEngine() {
  RegisterRule(std::make_unique<rules::TurnRule>());
  RegisterRule(std::make_unique<rules::ManaRule>());
  RegisterRule(std::make_unique<rules::TargetingRule>());
}

}  // namespace core::effects
