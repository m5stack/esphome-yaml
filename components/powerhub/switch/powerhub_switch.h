#pragma once

#include "esphome/components/switch/switch.h"
#include "../powerhub.h"
namespace esphome {
namespace powerhub {

class LEDSwitch : public switch_::Switch,
                  public Component,
                  public Parented<PowerHub>
{

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
        this->parent_->set_led_pwr_en(state);
        this->publish_state(state);
    }

};

class USBSwitch : public switch_::Switch,
                  public Component,
                  public Parented<PowerHub>
{
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
        this->parent_->set_usb_pwr_en(state);
        this->publish_state(state);
    }
};

class GroveRedSwitch : public switch_::Switch,
                       public Component,
                       public Parented<PowerHub>
{
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
        this->parent_->set_grove_red_pwr_en(state);
        this->publish_state(state);
    }
};

class GroveBlueSwitch : public switch_::Switch,
                        public Component,
                        public Parented<PowerHub>
{
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
        this->parent_->set_grove_blue_pwr_en(state);
        this->publish_state(state);
    }
};

class RS485CANSwitch : public switch_::Switch,
                       public Component,
                       public Parented<PowerHub>
{
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
        this->parent_->set_rs485_can_pwr_en(state);
        this->publish_state(state);
    }
};

class VAMeterSwitch : public switch_::Switch,
                      public Component,
                      public Parented<PowerHub>
{
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
        this->parent_->set_vameter_pwr_en(state);
        this->publish_state(state);
    }
};

class ChargeSwitch : public switch_::Switch,
                     public Component,
                     public Parented<PowerHub>
{
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
        this->parent_->set_charge_pwr_en(state);
        this->publish_state(state);
    }

};

class DirectionSwitch : public switch_::Switch,
                        public Component,
                        public Parented<PowerHub>
{
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
        this->parent_->set_rs485_can_pwr_output(state);
        this->parent_->set_rs485_can_pwr_direction(state);
        this->publish_state(state);
    }
};

} // namespace powerhub
} // namespace esphome
