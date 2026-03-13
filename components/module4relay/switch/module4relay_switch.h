#pragma once

#include "esphome/components/switch/switch.h"
#include "../module4relay.h"

namespace esphome {
namespace module4relay {

template<uint8_t RelayIndex>
class Module4RelaySwitch : public switch_::Switch,
                           public Component,
                           public Parented<Module4Relay> {
 public:
  void setup() override {
    bool initial_state = this->get_initial_state_with_restore_mode().value_or(false);
    this->write_state(initial_state);
  }
  void write_state(bool state) override {
    this->parent_->set_relay_by_index(RelayIndex, state);
    this->publish_state(state);
  }
};

using Module4RelaySwitch1 = Module4RelaySwitch<0>;
using Module4RelaySwitch2 = Module4RelaySwitch<1>;
using Module4RelaySwitch3 = Module4RelaySwitch<2>;
using Module4RelaySwitch4 = Module4RelaySwitch<3>;

}  // namespace module4relay
}  // namespace esphome
