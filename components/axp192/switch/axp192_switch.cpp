//#include "esphome/core/log.h"
#include "axp192_switch.h"

namespace esphome {
namespace axp192 {

static const char *TAG = "axp192.switch";

void AXP192Switch::setup() {
    bool initial_state = this->get_initial_state_with_restore_mode().value_or(false);
    if (initial_state) {
        this->turn_on();
    } else {
        this->turn_off();
    }
    ESP_LOGCONFIG(TAG, "AXP192 switch setup %X", initial_state);
}
void AXP192Switch::write_state(bool state) {
    //GPIO1 is connected to Green Sys LED
    this->parent_->setGpioPin(1, state);
    this->publish_state(state); //Critial line to publish state back to HA so switch stays in state (unless set as optimistic)
    ESP_LOGCONFIG(TAG, "AXP192 switch write state %X", state);
}
void AXP192Switch::dump_config(){
    ESP_LOGCONFIG(TAG, "AXP192 switch");
}

void VibrationSwitch::setup() {
    bool initial_state = this->get_initial_state_with_restore_mode().value_or(false);
    if (initial_state) {
        this->turn_on();
    } else {
        this->turn_off();
    }
    ESP_LOGCONFIG(TAG, "Vibration switch setup %X", initial_state);
}
void VibrationSwitch::write_state(bool state) {
    //GPIO1 is connected to Green Sys LED
    if (state) {
        this->parent_->enableDLDO3();
    } else {
        this->parent_->disableDLDO3();
    }
    this->publish_state(state); //Critial line to publish state back to HA so switch stays in state (unless set as optimistic)
    ESP_LOGCONFIG(TAG, "Vibration switch write state %X", state);
}
void VibrationSwitch::dump_config(){
    ESP_LOGCONFIG(TAG, "Vibration switch");
}

} //namespace axp192_switch
} //namespace esphome