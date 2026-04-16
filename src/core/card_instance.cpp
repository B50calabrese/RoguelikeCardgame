#include "core/card_instance.h"
#include "core/card_data.h"

namespace core {

CardInstance::CardInstance(const CardData* card_data, int inst_id, int owner)
    : data(card_data),
      instance_id(inst_id),
      owner_id(owner),
      location(CardLocation::Deck),
      current_cost(card_data->cost),
      current_power(card_data->power),
      current_health(card_data->health),
      max_health(card_data->health),
      has_attacked(false),
      can_attack(false) {}

}  // namespace core
