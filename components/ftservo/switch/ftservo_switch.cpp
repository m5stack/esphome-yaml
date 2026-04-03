#include "ftservo_switch.h"

namespace esphome {
namespace ftservo {

static const char *const TAG = "ftservo.switch";

void ServoTorqueSwitch::setup() {
  // Read initial state 
  bool initial_state = this->parent_->read_torque_enable();
  this->publish_state(initial_state);

}
void ServoTorqueSwitch::dump_config() {
  LOG_SWITCH("  ", "FTServo Torque Switch", this);
}
void ServoTorqueSwitch::write_state(bool state) {

  // write torque enable/disable
  this->parent_->write_torque_enable(state);
  
  // publish state
  this->publish_state(state);
}

}  // namespace ftservo
}  // namespace esphome