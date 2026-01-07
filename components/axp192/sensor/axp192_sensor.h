#pragma once
#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/binary_sensor/binary_sensor.h"
#include "../axp192.h"

namespace esphome {
namespace axp192 {

class AXP192Sensor : public sensor::Sensor,
                      public PollingComponent,
                      public Parented<AXP192>
{

public:
    void setup() override {};
    void loop() override {};
    void update() override;
    void dump_config() override;
    float get_setup_priority() const override { return setup_priority::DATA; }
    void set_battery_discharge_0pc_voltage(uint16_t voltage) { this->battery_discharge_0pc_Voltage_ = voltage; }
    void set_battery_discharge_100pc_voltage(uint16_t voltage) { this->battery_discharge_100pc_Voltage_ = voltage; }
    void set_battery_charge_0pc_current(uint16_t current) { this->battery_charge_0pc_Current_ = current; }
    void set_battery_charge_100pc_current(uint16_t current) { this->battery_charge_100pc_Current_ = current; }
    // Sensors
    void set_battery_level_sensor(sensor::Sensor *battery_level_sensor) { this->battery_level_sensor_ = battery_level_sensor; }
    void set_battery_voltage_sensor(sensor::Sensor *battery_voltage_sensor) { this->battery_voltage_sensor_ = battery_voltage_sensor; }
    void set_usb_voltage_sensor(sensor::Sensor *usb_voltage_sensor) { this->usb_voltage_sensor_ = usb_voltage_sensor; }
    void set_usb_current_sensor(sensor::Sensor *usb_current_sensor) { this->usb_current_sensor_ = usb_current_sensor; }
    void set_vbus_voltage_sensor(sensor::Sensor *vbus_voltage_sensor) { this->vbus_voltage_sensor_ = vbus_voltage_sensor; }
    void set_vbus_current_sensor(sensor::Sensor *vbus_current_sensor) { this->vbus_current_sensor_ = vbus_current_sensor; }
    void set_internal_temperature_sensor(sensor::Sensor *internal_temperature_sensor) { this->internal_temperature_sensor_ = internal_temperature_sensor; }
    void set_battery_power_sensor(sensor::Sensor *battery_power_sensor) { this->battery_power_sensor_ = battery_power_sensor; }
    void set_battery_charge_current_sensor(sensor::Sensor *battery_charge_current_sensor) { this->battery_charge_current_sensor_ = battery_charge_current_sensor; }
    void set_battery_discharge_current_sensor(sensor::Sensor *battery_discharge_current_sensor) { this->battery_discharge_current_sensor_ = battery_discharge_current_sensor; }
    void set_aps_voltage_sensor(sensor::Sensor *aps_voltage_sensor) { this->aps_voltage_sensor_ = aps_voltage_sensor; }
    void set_battery_charging_binary_sensor(binary_sensor::BinarySensor *battery_charging_binary_sensor) { this->battery_charging_binary_sensor_ = battery_charging_binary_sensor; }
    void set_usb_present_binary_sensor(binary_sensor::BinarySensor *usb_present_binary_sensor) { this->usb_present_binary_sensor_ = usb_present_binary_sensor; }
    void set_usb_valid_binary_sensor(binary_sensor::BinarySensor *usb_valid_binary_sensor) { this->usb_valid_binary_sensor_ = usb_valid_binary_sensor; }
    void set_vbus_present_binary_sensor(binary_sensor::BinarySensor *vbus_present_binary_sensor) { this->vbus_present_binary_sensor_ = vbus_present_binary_sensor; }
    void set_vbus_valid_binary_sensor(binary_sensor::BinarySensor *vbus_valid_binary_sensor) { this->vbus_valid_binary_sensor_ = vbus_valid_binary_sensor; }
    void set_battery_discharging_binary_sensor(binary_sensor::BinarySensor *battery_discharging_binary_sensor) { this->battery_discharging_binary_sensor_ = battery_discharging_binary_sensor; }
    void set_over_temp_binary_sensor(binary_sensor::BinarySensor *over_temp_binary_sensor) { this->over_temp_binary_sensor_ = over_temp_binary_sensor; }
    void set_battery_connected_binary_sensor(binary_sensor::BinarySensor *battery_connected_binary_sensor) { this->battery_connected_binary_sensor_ = battery_connected_binary_sensor; }
    void set_battery_active_mode_binary_sensor(binary_sensor::BinarySensor *battery_active_mode_binary_sensor) { this->battery_active_mode_binary_sensor_ = battery_active_mode_binary_sensor; }
    void set_low_charge_current_binary_sensor(binary_sensor::BinarySensor *low_charge_current_binary_sensor) { this->low_charge_current_binary_sensor_ = low_charge_current_binary_sensor; }

private:
    sensor::Sensor *battery_level_sensor_{nullptr};

    sensor::Sensor *usb_voltage_sensor_{nullptr};
    sensor::Sensor *usb_current_sensor_{nullptr};
    sensor::Sensor *vbus_voltage_sensor_{nullptr};
    sensor::Sensor *vbus_current_sensor_{nullptr};
    sensor::Sensor *internal_temperature_sensor_{nullptr};
    sensor::Sensor *battery_power_sensor_{nullptr};
    sensor::Sensor *battery_charge_current_sensor_{nullptr};
    sensor::Sensor *battery_discharge_current_sensor_{nullptr};
    sensor::Sensor *aps_voltage_sensor_{nullptr};
    sensor::Sensor *battery_voltage_sensor_{nullptr};
    sensor::Sensor *axp_temperature_sensor_{nullptr};

    binary_sensor::BinarySensor *battery_charging_binary_sensor_{nullptr};
    binary_sensor::BinarySensor *usb_present_binary_sensor_{nullptr};
    binary_sensor::BinarySensor *usb_valid_binary_sensor_{nullptr};
    binary_sensor::BinarySensor *vbus_present_binary_sensor_{nullptr};
    binary_sensor::BinarySensor *vbus_valid_binary_sensor_{nullptr};
    binary_sensor::BinarySensor *battery_discharging_binary_sensor_{nullptr};
    binary_sensor::BinarySensor *over_temp_binary_sensor_{nullptr};
    binary_sensor::BinarySensor *battery_connected_binary_sensor_{nullptr};
    binary_sensor::BinarySensor *battery_active_mode_binary_sensor_{nullptr};
    binary_sensor::BinarySensor *low_charge_current_binary_sensor_{nullptr};

protected:
    uint16_t battery_discharge_0pc_Voltage_{1005}; //mV Nte these default values are overridden by defaults in _init_py file
    uint16_t battery_discharge_100pc_Voltage_{3305}; //mV
    uint16_t battery_charge_0pc_Current_{435}; //mA
    uint16_t battery_charge_100pc_Current_{105}; //mA
};

}
}