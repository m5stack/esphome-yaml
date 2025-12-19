// Copyright (c) M5Stack. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.
// Original register reference:
// https://m5stack-doc.oss-cn-shenzhen.aliyuncs.com/1183/PowerHub-STM32-Protocol-CN-V1.0-20251024.pdf
// https://m5stack-doc.oss-cn-shenzhen.aliyuncs.com/1183/PowerHub_Register-Table-V2.pdf

#pragma once
#include "esphome/core/defines.h"
#include "esphome/core/component.h"
#include "esphome/components/i2c/i2c.h"

#ifdef USE_BINARY_SENSOR
#include "esphome/components/binary_sensor/binary_sensor.h"
#endif

#ifdef USE_SENSOR
#include "esphome/components/sensor/sensor.h"
#endif

#ifdef USE_TEXT_SENSOR
#include "esphome/components/text_sensor/text_sensor.h"
#endif

#ifdef USE_SWITCH
#include "esphome/components/switch/switch.h"
#endif

#ifdef USE_SELECT
#include "esphome/components/select/select.h"
#endif

#ifdef USE_NUMBER
#include "esphome/components/number/number.h"
#endif

namespace esphome {
namespace powerhub {


#define RETURN_IF_ERROR(expr)  \
    do {                       \
        if ((expr) != i2c::ERROR_OK) { \
            this->mark_failed(); \
            return;            \
        }                      \
    } while (0)

#define WARN_IF(expr)  \
    do {                       \
        if ((expr) != i2c::ERROR_OK) { \
            ESP_LOGW(TAG, "Error when read/write register."); \
        }                      \
    } while (0)

typedef struct BGR {
    uint8_t b;
    uint8_t g;
    uint8_t r;
} BGR_t;

typedef struct RTC {
    uint8_t rtc_sec;
    uint8_t rtc_min;
    uint8_t rtc_hour;
    uint8_t rtc_day;
    uint8_t rtc_month;
    uint8_t rtc_year;
    uint8_t rtc_weekday;
} RTC_t;

typedef struct RTC_Alarm {
    uint8_t alarm_min;
    uint8_t alarm_hour;
    uint8_t alarm_day;
} RTC_Alarm_t;

typedef struct Raw_Power {
    uint16_t bat_volt;
    int16_t  bat_curr;

    uint16_t can_volt;
    int16_t  can_curr;

    uint16_t rs485_volt;
    int16_t  rs485_curr;

    uint16_t usb_volt;
    int16_t  usb_curr;

    uint16_t grove_red_volt;
    int16_t  grove_red_curr;

    uint16_t grove_blue_volt;
    int16_t  grove_blue_curr;

} Raw_Power_t;

typedef struct Channel_Sample {
    uint16_t voltage{0};
    int16_t  current{0};
} Channel_Sample_t;


class PowerHub :  public PollingComponent, public i2c::I2CDevice {

#ifdef USE_BINARY_SENSOR
    SUB_BINARY_SENSOR(button)
#endif


#ifdef USE_SENSOR
    SUB_SENSOR(battery_voltage)
    SUB_SENSOR(battery_current)
    SUB_SENSOR(battery_level)
    SUB_SENSOR(grove_red_voltage)
    SUB_SENSOR(grove_red_current)
    SUB_SENSOR(grove_blue_voltage)
    SUB_SENSOR(grove_blue_current)
    SUB_SENSOR(can_voltage)
    SUB_SENSOR(can_current)
    SUB_SENSOR(rs485_voltage)
    SUB_SENSOR(rs485_current) 
    SUB_SENSOR(usb_voltage)
    SUB_SENSOR(usb_current)
#endif

#ifdef USE_TEXT_SENSOR
    SUB_TEXT_SENSOR(charge_status)
    SUB_TEXT_SENSOR(vin_status)
    SUB_TEXT_SENSOR(firmware_ver)
    SUB_TEXT_SENSOR(bootloader_ver)
#endif

#ifdef USE_SWITCH
    SUB_SWITCH(led_pwr)
    SUB_SWITCH(usb_pwr)
    SUB_SWITCH(grove_red_pwr)
    SUB_SWITCH(grove_blue_pwr)
    SUB_SWITCH(rs485_can_pwr)
    SUB_SWITCH(vameter_pwr)
    SUB_SWITCH(charge_pwr)
    SUB_SWITCH(rs485_can_direction)
#endif

#ifdef USE_SELECT
    SUB_SELECT(usb_mode)
#endif

#ifdef USE_NUMBER
    SUB_NUMBER(rs485_can_output_voltage)
    SUB_NUMBER(rs485_can_current_limit)
#endif

public:
    void setup() override;
    void update() override;
    void dump_config() override;

    float get_setup_priority() const override { return setup_priority::IO; }
  
    bool get_led_pwr_en() const { return this->led_power_enabled_; }
    void set_led_pwr_en(bool en);
    bool get_usb_pwr_en() const { return this->usb_power_enabled_; }
    void set_usb_pwr_en(bool en);
    bool get_grove_red_pwr_en() const { return this->grove_red_power_enabled_; }
    void set_grove_red_pwr_en(bool en);
    bool get_grove_blue_pwr_en() const { return this->grove_blue_power_enabled_; }
    void set_grove_blue_pwr_en(bool en);
    bool get_rs485_can_pwr_en() const { return this->rs485_can_power_enabled_; }
    void set_rs485_can_pwr_en(bool en);
    bool get_vameter_pwr_en() const { return this->vameter_power_enabled_; }
    void set_vameter_pwr_en(bool en);
    bool get_charge_pwr_en() const { return this->charge_power_enabled_; }
    void set_charge_pwr_en(bool en);

    // USB mode
    void set_usb_mode(uint8_t mode);

    // RS485 & CAN (SC8721) voltage, current and output
    uint16_t get_rs485_can_voltage() const { return this->rs485_can_voltage_mv_; }
    void set_rs485_can_voltage(uint16_t mv);

    uint16_t get_rs485_can_current() const { return this->rs485_can_current_limit_; }
    void set_rs485_can_current(uint16_t limit);

    uint8_t get_rs485_can_pwr_output() const { return this->rs485_can_power_output_ctrl_; }
    void set_rs485_can_pwr_output(bool output);

    uint8_t get_rs485_can_pwr_direction() const { return this->rs485_can_power_direction_; }
    void set_rs485_can_pwr_direction(bool direction);

    // INA266 VAMeters
    void read_power_channel();
    void update_vameter_sensor();

    float calc_battery_level();

    // Status detect
    uint8_t read_charge_status();
    uint8_t read_vin_status();

    // text sensor values used to indicate
    // if the battery is charging and
    // external input power is connected
    void update_charge_vin_sensor();

    // LED colors
    BGR_t get_led_usb_c_color() const { return this->led_usb_c_color_; }
    void set_led_usb_c_color(const BGR_t &c);

    BGR_t get_led_usb_a_color() const { return this->led_usb_a_color_; }
    void set_led_usb_a_color(const BGR_t &c);

    BGR_t get_led_grove_blue_color() const { return this->led_grove_blue_color_; }
    void set_led_grove_blue_color(const BGR_t &c);

    BGR_t get_led_rs485_can_color() const { return this->led_rs485_can_color_; }
    void set_led_rs485_can_color(const BGR_t &c);

    BGR_t get_led_grove_red_color() const { return this->led_grove_red_color_; }
    void set_led_grove_red_color(const BGR_t &c);

    BGR_t get_led_bat_charge_color() const { return this->led_bat_charge_color_; }
    void set_led_bat_charge_color(const BGR_t &c);

    BGR_t get_led_pwr_l_color() const { return this->led_pwr_l_color_; }
    void set_led_pwr_l_color(const BGR_t &c);

    BGR_t get_led_pwr_r_color() const { return this->led_pwr_r_color_; }
    void set_led_pwr_r_color(const BGR_t &c);

    // LED brightness
    uint8_t get_brightness_usb_c() const { return this->brightness_usb_c_; }
    void set_brightness_usb_c(uint8_t b);

    uint8_t get_brightness_usb_a() const { return this->brightness_usb_a_; }
    void set_brightness_usb_a(uint8_t b);

    uint8_t get_brightness_grove_blue() const { return this->brightness_grove_blue_; }
    void set_brightness_grove_blue(uint8_t b);

    uint8_t get_brightness_rs485_can() const { return this->brightness_rs485_can_; }
    void set_brightness_rs485_can(uint8_t b);

    uint8_t get_brightness_grove_red() const { return this->brightness_grove_red_; }
    void set_brightness_grove_red(uint8_t b);

    uint8_t get_brightness_bat_charge() const { return this->brightness_bat_charge_; }
    void set_brightness_bat_charge(uint8_t b);

    uint8_t get_brightness_pwr_l() const { return this->brightness_pwr_l_; }
    void set_brightness_pwr_l(uint8_t b);

    uint8_t get_brightness_pwr_r() const { return this->brightness_pwr_r_; }
    void set_brightness_pwr_r(uint8_t b);
    
    // button status
    bool read_pmu_btn_status();

    // To separate from other sensors
    // Because we need to get the button status fast
    void update_pmu_button_sensor();


    // RTC related
    // Wakeup getters/setters
    uint8_t get_wake_nstby() const { return this->wake_nstby_; }
    void set_wake_nstby(bool en);
    uint8_t get_wake_vin_detect() const { return this->wake_vin_detect_; }
    void set_wake_vin_detect(bool en);
    uint8_t get_wake_vusb_detect() const { return this->wake_vusb_detect_; }
    void set_wake_vusb_detect(bool en);

    // RTC getters/setters
    RTC_t get_rtc_time() { return this->clock_; };
    void set_rtc_time(const RTC_t &t);

    // Alarm getters/setters
    RTC_Alarm_t get_rtc_alarm() { return this->rtc_alarm_; };
    void set_rtc_alarm(const RTC_Alarm_t &alarm);
    bool get_alarm_enabled() const { return this->alarm_enabled_; }
    void set_alarm_enabled(bool en);

    // System command
    void sys_poweroff();
    void sys_reset();
    void sys_download_mode();

    // bootloader & firmware
    uint8_t get_bootloader_ver() const { return this->boot_version_; }
    uint8_t get_firmware_ver() const { return this->firmware_version_; }

    // i2c address
    uint8_t get_i2c_addr() const { return this->i2c_address_; }

protected:
    // Power enable
    bool led_power_enabled_;
    bool usb_power_enabled_;
    bool grove_red_power_enabled_;
    bool grove_blue_power_enabled_;
    bool rs485_can_power_enabled_;
    bool vameter_power_enabled_;
    bool charge_power_enabled_;


    // Used to store the last PMU button state
    bool last_button_state_{false};

    // USB mode
    uint8_t usb_mode_;

    // RS485/CAN
    uint16_t rs485_can_voltage_mv_;
    uint16_t  rs485_can_current_limit_;
    bool  rs485_can_power_output_ctrl_;
    bool  rs485_can_power_direction_;

    // voltage / current readings from all power channel
    Raw_Power_t power_;

    // voltage / current sample from channel
    Channel_Sample_t usb_;
    Channel_Sample_t grove_red_;
    Channel_Sample_t grove_blue_;
    Channel_Sample_t can_;
    Channel_Sample_t rs485_;
    Channel_Sample_t battery_;

    // Status
    uint8_t charge_status_;
    uint8_t vin_status_;

    // LED colors
    BGR_t led_usb_c_color_;
    BGR_t led_usb_a_color_;
    BGR_t led_grove_blue_color_;
    BGR_t led_rs485_can_color_;
    BGR_t led_grove_red_color_;
    BGR_t led_bat_charge_color_;
    BGR_t led_pwr_l_color_;
    BGR_t led_pwr_r_color_;

    // Brightness
    uint8_t brightness_usb_c_;
    uint8_t brightness_usb_a_;
    uint8_t brightness_grove_blue_;
    uint8_t brightness_rs485_can_;
    uint8_t brightness_grove_red_;
    uint8_t brightness_bat_charge_;
    uint8_t brightness_pwr_l_;
    uint8_t brightness_pwr_r_;

    // Button
    bool button_pressed_;

    // wakeup enable
    bool wake_nstby_;
    bool wake_vin_detect_;
    bool wake_vusb_detect_;

    // RTC
    RTC_t clock_;

    // Alarm
    RTC_Alarm_t rtc_alarm_;
    bool alarm_enabled_;

    // Versions
    uint8_t boot_version_;
    uint8_t firmware_version_;

    // I2C addr
    uint8_t i2c_address_;    

};


} // namespace powerhub
} // namespace esphome