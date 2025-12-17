#pragma once

#include "esphome/components/select/select.h"
#include "../powerhub.h"


namespace esphome {
namespace powerhub {

class USBModeSelect : public select::Select,
                       public Component,
                       public Parented<PowerHub>
{

public:
    void control(const std::string &value) override {
        this->parent_->set_usb_mode(this->index_of(value).value());
        this->publish_state(value);
    }


};
} // namespace powerhub
} // namespace esphome
