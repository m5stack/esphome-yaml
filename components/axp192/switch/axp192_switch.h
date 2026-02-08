#pragma once

#include "esphome/core/component.h" //handled at axp192 level
#include "esphome/components/switch/switch.h"
#include "../axp192.h"

namespace esphome {
namespace axp192 {

class AXP192Switch : public switch_::Switch,
                     public Component,
                     public Parented<AXP192>
{
public:
    void setup() override;
    void write_state(bool state) override; 
    void dump_config() override;
};

class VibrationSwitch : public switch_::Switch,
                     public Component,
                     public Parented<AXP192>
{
public:
    void setup() override;
    void write_state(bool state) override; 
    void dump_config() override;
};



} //namespace axp192
} //namespace esphome