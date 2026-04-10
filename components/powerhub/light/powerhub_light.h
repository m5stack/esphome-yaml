#pragma once

#include "esphome/components/light/light_output.h"
#include "esphome/components/light/light_effect.h"
#include "esphome/components/light/light_state.h"
#include "../powerhub.h"

namespace esphome {
namespace powerhub {

enum class LightChannel {
  LED_USB_A,
  LED_USB_C,
  LED_GROVE_RED,
  LED_GROVE_BLUE,
  LED_RS485_CAN,
  LED_CHARGE,
  LED_POWER_LEFT,
  LED_POWER_RIGHT
};

class PowerHubLight : public light::LightOutput,
                      public Parented<PowerHub>
{
 public:
  light::LightTraits get_traits() override;

  void set_channel(LightChannel channel) { this->channel_ = channel; }

  void write_state(light::LightState *state) override;

 protected:
  LightChannel channel_;
};                      
    
} // namespace powerhub
} // namespace esphome
