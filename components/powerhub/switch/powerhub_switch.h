#pragma once

#include "esphome/components/switch/switch.h"
#include "../powerhub.h"



namespace esphome {
namespace powerhub {

enum class PowerChannel {
  POWER_LED,
  POWER_USB,
  POWER_GROVE_RED,
  POWER_GROVE_BLUE,
  POWER_RS485_CAN,
  POWER_VAMETER,
  POWER_CHARGE,
};


class PowerSwitch : public switch_::Switch,
                    public Component,
                    public Parented<PowerHub>
{
 public:
  void set_channel(PowerChannel channel) { this->channel_ = channel; }
  void setup() override;
  void dump_config() override;
 protected:
  void write_state(bool state) override;
  PowerChannel channel_;
};


class RS485CANDirSwitch : public switch_::Switch,
                          public Component,
                          public Parented<PowerHub>
{
 public:
  void setup() override;
  void dump_config() override;
 protected:
  void write_state(bool state) override;
};

} // namespace powerhub
} // namespace esphome
