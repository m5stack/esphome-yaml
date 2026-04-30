#pragma once

#include "esphome/components/light/addressable_light.h"
#include "esphome/components/light/light_output.h"
#include "../extio2.h"
#include "esphome/components/light/light_state.h"
#include "esphome/core/helpers.h"
#include <cstdint>


namespace esphome {
namespace extio2 {

enum EXTIO2LightChannel {
  LIGHT_0 = 0,
  LIGHT_1 = 1,
  LIGHT_2 = 2,
  LIGHT_3 = 3,
  LIGHT_4 = 4,
  LIGHT_5 = 5,
  LIGHT_6 = 6,
  LIGHT_7 = 7
};

class EXTIO2Light : public light::LightOutput,
                    public Component,
                    public Parented<EXTIO2Component>
{
 public:
  void setup() override;
  void dump_config() override;
  light::LightTraits get_traits() override;
  void write_state(light::LightState *state) override;
  float get_setup_priority() const override { return setup_priority::HARDWARE; }
  void set_light_channel(EXTIO2LightChannel channel) { this->channel_ = channel; }
 protected:
  EXTIO2LightChannel channel_;
};

class EXTIO2AddressableLight : public light::AddressableLight,
                               public Parented<EXTIO2Component>
{
 public:
  void setup() override;
  void dump_config() override;
  void write_state(light::LightState *state) override;
  float get_setup_priority() const override { return setup_priority::HARDWARE; }
  void set_num_leds(int32_t num) { this->num_leds_ = num; }
  light::LightTraits get_traits() override;
  int32_t size() const override { return this->num_leds_; }

  void clear_effect_data() override;
 protected:
  light::ESPColorView get_view_internal(int32_t index) const override; 
  int32_t num_leds_;
  uint8_t *buf_{nullptr};
  uint8_t *effect_data_{nullptr};
};


}  // namespace extio2
}  // namespace esphome