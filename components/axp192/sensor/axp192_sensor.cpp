#include "axp192_sensor.h"

namespace esphome {
namespace axp192 {

static const char *TAG = "axp192.sensor";

void AXP192Sensor::update() {

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



void AXP192Sensor::dump_config() {
    ESP_LOGCONFIG(TAG, "AXP192 Sensor: ");
}

}
}