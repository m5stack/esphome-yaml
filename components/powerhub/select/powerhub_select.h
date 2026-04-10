#pragma once

#include "esphome/components/select/select.h"
#include "../powerhub.h"

namespace esphome {
namespace powerhub {

class USBModeSelect : public select::Select,
                       public Parented<PowerHub>
{
 public:
  void control(const std::string &value) override {
    auto index = this->index_of(value);
    if (index.has_value()) {
      this->parent_->set_usb_mode(static_cast<uint8_t>(index.value()));
      this->publish_state(value);
    }
  }
};
} // namespace powerhub
} // namespace esphome
