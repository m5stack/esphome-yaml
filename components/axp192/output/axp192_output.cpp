#include "axp192_output.h"
#include <map>
namespace esphome {
namespace axp192 {

static const char *TAG = "axp192.output";

void AXP192OutputBase::apply_channel(PowerChannel channel, uint16_t voltage) {
  switch (channel) {
    // case PowerChannel::ALDO1:
    //   this->parent_->setALDO1Voltage(voltage);
    //   this->parent_->enableALDO1();
    //   break;
    // case PowerChannel::ALDO2:
    //   this->parent_->setALDO2Voltage(voltage);
    //   this->parent_->enableALDO2();
    //   break;
    // case PowerChannel::ALDO3:
    //   this->parent_->setALDO3Voltage(voltage);
    //   this->parent_->enableALDO3();
    //   break;
    case PowerChannel::DLDO3:
      this->parent_->setDLDO3Voltage(voltage);
      this->parent_->enableDLDO3();
      break;
    case PowerChannel::DLDO2:
      this->parent_->setDLDO2Voltage(voltage);
      this->parent_->enableDLDO2();
      break;
    case PowerChannel::DCDC1:
      this->parent_->setDC1Voltage(voltage);
      this->parent_->enableDC1();
      break;
    case PowerChannel::DCDC2:
      this->parent_->setDC2Voltage(voltage);
      this->parent_->enableDC2();
      break;
    case PowerChannel::DCDC3:
      this->parent_->setDC3Voltage(voltage);
      this->parent_->enableDC3();
      break;
    default:
      ESP_LOGW(TAG, "Unknown channel: %d", channel);
      break;
  }
}

void AXP192OutputBase::disable_channel(PowerChannel channel) {
  switch (channel) {
    // case PowerChannel::ALDO1: this->parent_->disableALDO1(); break;
    // case PowerChannel::ALDO2: this->parent_->disableALDO2(); break;
    // case PowerChannel::ALDO3: this->parent_->disableALDO3(); break;
    case PowerChannel::DLDO3: this->parent_->disableDLDO3(); break;
    case PowerChannel::DLDO2: this->parent_->disableDLDO2(); break;
    case PowerChannel::DCDC1: this->parent_->disableDC1(); break;
    case PowerChannel::DCDC2: this->parent_->disableDC2(); break;
    case PowerChannel::DCDC3: this->parent_->disableDC3(); break;
    default:
      ESP_LOGW(TAG, "Unknown channel: %d", channel);
      break;
  }
}

void AXP192FloatOutput::write_state(float state) {

    // voltage normalization with the given step
    uint16_t raw_voltage = static_cast<uint16_t>(min_voltage_ + state * (max_voltage_ - min_voltage_));
    uint16_t aligned_voltage = (raw_voltage / this->step_) * this->step_;

    // voltage boundary value
    if (aligned_voltage < min_voltage_) aligned_voltage = min_voltage_;
    if (aligned_voltage > max_voltage_) aligned_voltage = max_voltage_;

    // if state is 0.0f, turn off corresponding power channel
    if ( state == 0.0f ) {
        this->disable_channel(this->channel_);
        return ;
    }

    this->apply_channel(this->channel_, aligned_voltage);
}


void AXP192BinaryOutput::write_state(bool state) {
    if ( state ) {
        this->apply_channel(this->channel_, this->voltage_);
    } else {
        this->disable_channel(this->channel_);
    }
}

} // axp192
} // esphome