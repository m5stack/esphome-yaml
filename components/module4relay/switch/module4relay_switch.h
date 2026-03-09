#pragma once

#include "esphome/components/switch/switch.h"
#include "../module4relay.h"

namespace esphome {
namespace module4relay {

class Module4RelaySwitch1 : public switch_::Switch,
                            public Component,
                            public Parented<Module4Relay> {
 public:
  void setup() override {
    bool initial_state = this->get_initial_state_with_restore_mode().value_or(false);
    if (initial_state) {
      this->turn_on();
    } else {
      this->turn_off();
    }
  }
  void write_state(bool state) override {
    this->parent_->set_relay_by_index(0, state);
    this->publish_state(state);
  }
};

class Module4RelaySwitch2 : public switch_::Switch,
                            public Component,
                            public Parented<Module4Relay> {
 public:
  void setup() override {
    bool initial_state = this->get_initial_state_with_restore_mode().value_or(false);
    if (initial_state) {
      this->turn_on();
    } else {
      this->turn_off();
    }
  }
  void write_state(bool state) override {
    this->parent_->set_relay_by_index(1, state);
    this->publish_state(state);
  }
};

class Module4RelaySwitch3 : public switch_::Switch,
                            public Component,
                            public Parented<Module4Relay> {
 public:
  void setup() override {
    bool initial_state = this->get_initial_state_with_restore_mode().value_or(false);
    if (initial_state) {
      this->turn_on();
    } else {
      this->turn_off();
    }
  }
  void write_state(bool state) override {
    this->parent_->set_relay_by_index(2, state);
    this->publish_state(state);
  }
};

class Module4RelaySwitch4 : public switch_::Switch,
                            public Component,
                            public Parented<Module4Relay> {
 public:
  void setup() override {
    bool initial_state = this->get_initial_state_with_restore_mode().value_or(false);
    if (initial_state) {
      this->turn_on();
    } else {
      this->turn_off();
    }
  }
  void write_state(bool state) override {
    this->parent_->set_relay_by_index(3, state);
    this->publish_state(state);
  }
};

}  // namespace module4relay
}  // namespace esphome
