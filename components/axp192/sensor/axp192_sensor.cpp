#include "axp192_sensor.h"

namespace esphome {
namespace axp192 {

static const char *TAG = "axp192.sensor";

void AXP192Sensor::update() {

    float reading = 0;
    int percent = this->parent_->getBatteryPercent();
    
    if (battery_level_sensor_ != nullptr) {
        battery_level_sensor_->publish_state(percent == -1 ? NAN : percent);
    }

    if(battery_voltage_sensor_ != nullptr) {
        reading = this->parent_->getBattVoltage();
        battery_voltage_sensor_->publish_state(reading == 0 ? NAN : reading);
    }

    if(usb_voltage_sensor_ != nullptr) {
        reading = this->parent_->getACINVoltage();
        usb_voltage_sensor_->publish_state(reading == 0 ? NAN : reading);
    }

    if(usb_current_sensor_ != nullptr) {
        reading = this->parent_->getACINCurrent();
        usb_current_sensor_->publish_state(reading == 0 ? NAN : reading);
    }

    if(vbus_voltage_sensor_ != nullptr) {
        reading = this->parent_->getACINVoltage();
        vbus_voltage_sensor_->publish_state(reading == 0 ? NAN : reading);
    }

    if(vbus_current_sensor_ != nullptr) {
        reading = this->parent_->getACINCurrent();
        vbus_current_sensor_->publish_state(reading == 0 ? NAN : reading);
    }

    if(internal_temperature_sensor_ != nullptr) {
        reading = this->parent_->getInternalTemperature();
        internal_temperature_sensor_->publish_state(reading == 0 ? NAN : reading);
    }

    if(battery_power_sensor_ != nullptr) {
        reading = this->parent_->getBattPower();
        battery_power_sensor_->publish_state(reading == 0 ? NAN : reading);
    }

    if(battery_charge_current_sensor_ != nullptr) {
        reading = this->parent_->getBattChargeCurrent();
        battery_charge_current_sensor_->publish_state(reading == 0 ? NAN : reading);
    }

    if(battery_discharge_current_sensor_ != nullptr) {
        reading = this->parent_->getBattDischargeCurrent();
        battery_discharge_current_sensor_->publish_state(reading == 0 ? NAN : reading);
    }

    if(aps_voltage_sensor_ != nullptr) {
        reading = this->parent_->getAPSVoltage();
        aps_voltage_sensor_->publish_state(reading == 0 ? NAN : reading);
    }

    if(battery_charging_binary_sensor_ != nullptr) {
        battery_charging_binary_sensor_->publish_state(this->parent_->isCharging());
    }

    this->status_clear_warning();
}



void AXP192Sensor::dump_config() {
    ESP_LOGCONFIG(TAG, "AXP192 Sensor: ");
}

}
}