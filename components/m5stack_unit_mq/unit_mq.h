
/*
 * unit_mq.h
 * Author: M5Stack
 * License: MIT
 * Copyright (c) 2025 M5Stack
 * Refer to the datasheet and communication protocols
 * https://m5stack-doc.oss-cn-shenzhen.aliyuncs.com/1168/Gas_Sensor_MQ-5_datasheet.pdf
 * https://m5stack-doc.oss-cn-shenzhen.aliyuncs.com/1168/MQ_I2C_Protocol_EN.pdf
 * 
*/


#pragma once

#include "esphome/core/component.h"
#include "esphome/components/i2c/i2c.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/binary_sensor/binary_sensor.h"

namespace esphome {
namespace unit_mq {

enum HeatMode {
    OFF,
    CONTINUOUS,
    SWITCH
};

class UnitMQSensor : public i2c::I2CDevice, 
                        public sensor::Sensor, 
                        public PollingComponent 
{    
    
public:
    void setup() override;
    void update() override;
    void dump_config() override;

    uint8_t get_mq_status() const { return this->mq_status_; }
    uint8_t get_led_status() const { return this->led_status_; }

    void set_mq_status(uint8_t val) { this->mq_status_ = val; }
    void set_led_status(uint8_t val) { this->led_status_ = val; }
    
    // high/low level timing
    uint8_t get_high_level_time() const { return this->high_level_time_; }
    void set_high_level_time(uint8_t val) { this->high_level_time_ = val; }

    uint8_t get_low_level_time() const { return this->low_level_time_; }
    void set_low_level_time(uint8_t val) { this->low_level_time_ = val; }

    // MQ ADC values
    uint8_t get_mq_adc_val_8() const { return this->mq_adc_val_8_; }
    uint16_t get_mq_adc_val_12() const { return this->mq_adc_val_12_; }

    // tags
    bool get_valid_tags() const { return this->valid_tags_; }

    // NTC ADC / resistance / voltages
    uint8_t get_ntc_adc_val_8() const { return this->ntc_adc_val_8_; }
    uint16_t get_ntc_adc_val_12() const { return this->ntc_adc_val_12_; }
    uint16_t get_ntc_resistance() const { return this->ntc_resistance_; }
    uint16_t get_ref_voltage() const { return this->ref_voltage_; }
    uint16_t get_mq_voltage() const { return this->mq_voltage_; }
    uint16_t get_ntc_voltage() const { return this->ntc_voltage_; }

    // firmware
    uint8_t get_firmware_version() const { return this->firm_ver_; }
    
    void set_heat_mode(HeatMode mode) { this->mode_ = mode; }

    SUB_SENSOR(temperature)
    SUB_SENSOR(reference_voltage)
    SUB_SENSOR(mq_voltage)
    SUB_SENSOR(ntc_voltage)
    SUB_SENSOR(ntc_resistance)
    SUB_SENSOR(mq_adc)
    SUB_SENSOR(ntc_adc)
    SUB_BINARY_SENSOR(led)

protected:
    void update_temperature_sensor();
    void update_mq_adc_sensor();
    void update_ntc_adc_sensor();
    void update_reference_voltage_sensor();
    void update_mq_voltage_sensor();
    void update_ntc_voltage_sensor();
    void update_led_binary_sensor();
    void update_ntc_resistance_sensor();

    uint8_t mq_status_{0};
    uint8_t led_status_{0};

    uint8_t high_level_time_{0};
    uint8_t low_level_time_{0};

    uint8_t mq_adc_val_8_;      // 8 bits MQ ADC
    uint16_t mq_adc_val_12_;    // 12 bits MQ ADC

    bool valid_tags_{false};

    uint8_t ntc_adc_val_8_;     // 8 bits NTC ADC
    uint16_t ntc_adc_val_12_;   // 12 bits NTC ADC 
    
    uint16_t ntc_resistance_;

    uint16_t ref_voltage_{3300};
    uint16_t mq_voltage_;
    uint16_t ntc_voltage_;

    uint8_t firm_ver_;

    HeatMode mode_;

};


} // namespace unit_mq
    
} // namespace esphome
