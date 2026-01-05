// Copyright (c) M5Stack. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.
// Original register reference:
// https://docs.m5stack.com/en/unit/4relay

#pragma once
#include "esphome/core/defines.h"
#include "esphome/components/i2c/i2c.h"
#include "esphome/core/component.h"

#ifdef USE_SWITCH
#include "esphome/components/switch/switch.h"
#endif


namespace esphome {
namespace unit4relay {
    
class Unit4Relay : public i2c::I2CDevice,
                   public Component
{

#ifdef USE_SWITCH
SUB_SWITCH(relay_1)
SUB_SWITCH(relay_2)
SUB_SWITCH(relay_3)
SUB_SWITCH(relay_4)
#endif

public:
    void setup() override;
    void dump_config() override;

    uint8_t read_all_relay();
    void set_all_relay(bool en);
    void set_relay_by_index(uint8_t index, bool en);

};


} // namespace unit4relay
} // namespace esphome
