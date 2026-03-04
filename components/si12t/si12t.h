#pragma once

#include "esphome/components/i2c/i2c.h"
#include "esphome/core/component.h"
#include <cstdint>

namespace esphome {
namespace si12t {

enum SI12TFirstTouchTime : uint8_t {
  FTT_5S = 0x00,
  FTT_10S = 0x01,
  FTT_15S = 0x02,
  FTT_20S = 0x03
};

enum SI12TInterruptLevel : uint8_t {
  OUTPUT_MEDIUM_HIGH = 0x00,
  OUTPUT_ALL = 0x01,
  OUTPUT_MEDIUM_HIGH_ALT = 0x02,
  OUTPUT_HIGH = 0x03
};

enum SI12TResponseTime : uint8_t {
  RES_CYCLE_2 = 0x00,
  RES_CYCLE_3 = 0x01,
  RES_CYCLE_4 = 0x02,
  RES_CYCLE_5 = 0x03,
  RES_CYCLE_6 = 0x04,
  RES_CYCLE_7 = 0x05,
  RES_CYCLE_8 = 0x06,
  RES_CYCLE_9 = 0x07
};

enum SI12TSensitivityLevel : uint8_t {
  SENS_LEVEL_0 = 0x00,
  SENS_LEVEL_1 = 0x01,
  SENS_LEVEL_2 = 0x02,
  SENS_LEVEL_3 = 0x03,
  SENS_LEVEL_4 = 0x04,
  SENS_LEVEL_5 = 0x05,
  SENS_LEVEL_6 = 0x06,
  SENS_LEVEL_7 = 0x07
};

enum SI12TChannel : uint8_t {
  CH_1 = 0,
  CH_2 = 1,
  CH_3 = 2,
  CH_4 = 3,
  CH_5 = 4,
  CH_6 = 5,
  CH_7 = 6,
  CH_8 = 7,
  CH_9 = 8,
  CH_10 = 9,
  CH_11 = 10,
  CH_12 = 11
};

class SI12TComponent : public Component,
                       public i2c::I2CDevice 
{

public:
  void setup() override;
  void loop() override;
  void dump_config() override;

  // configuration setters
  void set_auto_mode(bool mode) { this->auto_mode_ = mode; }
  void set_first_touch_time(SI12TFirstTouchTime ftc) { this->ftc_ = ftc; }
  void set_intr_level(SI12TInterruptLevel ilc) { this->ilc_ = ilc; }
  void set_res_time(SI12TResponseTime rtc) { this->rtc_ = rtc; }
  void set_reset(bool reset) { this->reset_ = reset; }
  void set_sleep_mode(bool sleep_mode) { this->sleep_mode_ = sleep_mode; }

  void setup_channel(SI12TChannel channel);
  void write_channel_sensitivity(SI12TChannel channel, SI12TSensitivityLevel sensitivity, bool extend_threshold);
  uint8_t read_channel_output(SI12TChannel channel);

protected:

  // Bad namings from datasheet ...
  // Register Configuration Parameters
  // Name  Description
  // mode  operation mode, 0: autoselect, 1: fast mode
  // ftc   first touch: 00=5s, 01=10s, 10=15s, 11=20s
  // ilc   interrupt level: 00/01/10/11 variants
  // rtc   response cycle = value + 2, range [2-9]
  bool auto_mode_{false};
  SI12TFirstTouchTime ftc_{FTT_10S};
  SI12TInterruptLevel ilc_{OUTPUT_MEDIUM_HIGH};
  SI12TResponseTime rtc_{RES_CYCLE_5};

  bool reset_{true}; // reset
  bool sleep_mode_{true};  // sleep mode

};

} // namespace si12t
} // namespace esphome