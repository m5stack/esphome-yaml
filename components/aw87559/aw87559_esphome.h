#pragma once

#include "esphome/core/component.h"
#include "esphome/components/i2c/i2c.h"
#include "esphome/core/gpio.h"
#include "esphome/core/hal.h"
#include "esphome/core/log.h"
#include <cinttypes>

namespace esphome {
namespace aw87559 {

static const uint8_t AW87559_REG_CHIPID = 0x00;
static const uint8_t AW87559_REG_SYSCTRL = 0x01;
static const uint8_t AW87559_REG_BATSAFE = 0x02;
static const uint8_t AW87559_REG_BSTOVR = 0x03;
static const uint8_t AW87559_REG_BSTVPR = 0x04;
static const uint8_t AW87559_REG_PAGR = 0x05;
static const uint8_t AW87559_REG_PAGC3OPR = 0x06;
static const uint8_t AW87559_REG_PAGC3PR = 0x07;
static const uint8_t AW87559_REG_PAGC2OPR = 0x08;
static const uint8_t AW87559_REG_PAGC2PR = 0x09;
static const uint8_t AW87559_REG_PAGC1PR = 0x0A;

static const uint8_t AW87559_CHIPID = 0x5A;

class AW87559Component : public esphome::Component, public esphome::i2c::I2CDevice {
 public:
  void setup() override;
  void dump_config() override;
  float get_setup_priority() const override { return esphome::setup_priority::HARDWARE; }
  bool setup_complete_{false};
};

}  // namespace aw87559
}  // namespace esphome
