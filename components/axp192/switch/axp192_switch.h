#pragma once

//#include "esphome/core/component.h" //handled at axp192 level
#include "esphome/components/switch/switch.h"
#include "../axp192.h"

namespace esphome {
namespace axp192{

class AXP192Switch : public switch_::Switch,
                     public Component,
                     public Parented<AXP192>
{
public:
    void setup() override;
    void write_state(bool state) override; 
    // {
    //     this->parent_->set_led_pwr_en(state);
    //     this->publish_state(state); //Critial line to publish state back to HA so switch stays in state (unless set as optimistic)
    // }
    void dump_config() override;
};



} //namespace axp192
} //namespace esphome