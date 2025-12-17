#pragma once

#include "esphome/components/number/number.h"
#include "../powerhub.h"


namespace esphome {
namespace powerhub {

class RS485CANOutputVoltageNumber : public number::Number,
                                    public Parented<PowerHub>
{
public:
    void control(float value) override {
        // The value maybe rounded or clamped.
        this->parent_->set_rs485_can_voltage(static_cast<uint16_t>(value));
        // publish the final value
        float final_val = static_cast<float>(this->parent_->get_rs485_can_voltage());
        this->publish_state(final_val);
    }

};


class RS485CANOutputCurrentNumber : public number::Number,
                                    public Parented<PowerHub>
{

public:
    void control(float value) override {
        this->parent_->set_rs485_can_current(static_cast<uint16_t>(value));
        float final_val = static_cast<float>(this->parent_->get_rs485_can_current());
        this->publish_state(final_val);
    }

};
    
} // namespace powerhub    
} // namespace esphome
