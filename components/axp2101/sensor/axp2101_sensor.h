#pragma once
#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/binary_sensor/binary_sensor.h"
#include "../axp2101.h"

namespace esphome {
namespace axp2101 {

class AXP2101Sensor : public sensor::Sensor,
                      public PollingComponent,
                      public Parented<AXP2101>
{

public:
    void setup() override {};
    void loop() override {};
    void update() override;
    void dump_config() override;
    float get_setup_priority() const override { return setup_priority::DATA; }
    // Sensors
    void set_battery_level_sensor(sensor::Sensor *battery_level_sensor) { this->battery_level_sensor_ = battery_level_sensor; }
    void set_axp_temperature_sensor(sensor::Sensor *axp_temperature_sensor) { this->axp_temperature_sensor_ = axp_temperature_sensor; }
    void set_battery_voltage_sensor(sensor::Sensor *battery_voltage_sensor) { this->battery_voltage_sensor_ = battery_voltage_sensor; }
    void set_battery_charging_binary_sensor(binary_sensor::BinarySensor *battery_charging_binary_sensor) { this->battery_charging_binary_sensor_ = battery_charging_binary_sensor; }

private:
    sensor::Sensor *battery_level_sensor_{nullptr};
    sensor::Sensor *battery_voltage_sensor_{nullptr};
    sensor::Sensor *axp_temperature_sensor_{nullptr};
    binary_sensor::BinarySensor *battery_charging_binary_sensor_{nullptr};
};

}
}