#pragma once

#include "esphome/components/i2c/i2c.h"
#include "esphome/core/component.h"

namespace esphome {
namespace aw9523b {

class AW9523B : public Component, public i2c::I2CDevice {

public:
    void setup() override;
    void loop() override;
    void dump_config() override;
};

}



}