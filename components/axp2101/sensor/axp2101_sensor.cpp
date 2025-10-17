#include "axp2101_sensor.h"

namespace esphome {
namespace axp2101 {

static const char *TAG = "axp2101.sensor";

void AXP2101Sensor::update() {

    int percent = this->parent_->getBatteryPercent();
    
    if (battery_level_sensor_ != nullptr) {
        battery_level_sensor_->publish_state(percent == -1 ? NAN : percent);
    }
    
    if(axp_temperature_sensor_ != nullptr) {
        axp_temperature_sensor_->publish_state(this->parent_->getTemperature());
    }

    uint16_t voltage = this->parent_->getBattVoltage();
    if(battery_voltage_sensor_ != nullptr) {
        battery_voltage_sensor_->publish_state(voltage == 0 ? NAN : voltage);
    }

    if(battery_charging_binary_sensor_ != nullptr) {
        battery_charging_binary_sensor_->publish_state(this->parent_->isCharging());
    }

    this->status_clear_warning();
}



void AXP2101Sensor::dump_config() {
    ESP_LOGCONFIG(TAG, "AXP2101 Sensor: ");
}

}
}