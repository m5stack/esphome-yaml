#include "axp192_sensor.h"

namespace esphome {
namespace axp192 {

static const char *TAG = "axp192.sensor";

void AXP192Sensor::update() {

    float reading = 0;
    int percent = 0;
    
    if (battery_level_sensor_ != nullptr) {
        percent = this->parent_->getBatteryPercent();
        percent = this->parent_->getScaledBatteryPercent(this->battery_discharge_0pc_Voltage_ , this->battery_discharge_100pc_Voltage_ , this->battery_charge_0pc_Current_ , this->battery_charge_100pc_Current_);
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

    if(usb_present_binary_sensor_ != nullptr) {
        usb_present_binary_sensor_->publish_state(this->parent_->isACINPresent());
    }

    if(usb_valid_binary_sensor_ != nullptr) {
        usb_valid_binary_sensor_->publish_state(this->parent_->isACINValid());
    }

    if(vbus_present_binary_sensor_ != nullptr) {
        vbus_present_binary_sensor_->publish_state(this->parent_->isVbusIn());
    }

    if(vbus_valid_binary_sensor_ != nullptr) {
        vbus_valid_binary_sensor_->publish_state(this->parent_->isVbusGood());
    }

    if(battery_discharging_binary_sensor_ != nullptr) {
        battery_discharging_binary_sensor_->publish_state(this->parent_->isDischarge());
    }

    if(over_temp_binary_sensor_ != nullptr) {
        over_temp_binary_sensor_->publish_state(this->parent_->getThermalRegulationStatus());
    }

    if(battery_connected_binary_sensor_ != nullptr) {
        battery_connected_binary_sensor_->publish_state(this->parent_->isBatteryConnect());
    }

    if(battery_active_mode_binary_sensor_ != nullptr) {
        battery_active_mode_binary_sensor_->publish_state(this->parent_->isBatInActiveModeState());
    }

    if(low_charge_current_binary_sensor_ != nullptr) {
        low_charge_current_binary_sensor_->publish_state(this->parent_->isChargingCurrentLessThanExpected());
    }

    this->status_clear_warning();
}



void AXP192Sensor::dump_config() {
    ESP_LOGCONFIG(TAG, "AXP192 Sensor: ");
}

}
}