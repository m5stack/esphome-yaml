#pragma once

#include "esphome/components/output/float_output.h"
#include "esphome/components/output/binary_output.h"
#include "../axp192.h"

namespace esphome {
namespace axp192{

enum PowerChannel {
    DCDC1,
    DCDC2,
    DCDC3,
    ALDO1,
    ALDO2,
    ALDO3,
    ALDO4,
    DLDO2,
    DLDO3,
};

class AXP192OutputBase : public Parented<AXP192> {
public:
    void apply_channel(PowerChannel channel, uint16_t voltage);
    void disable_channel(PowerChannel channel);
};

class AXP192FloatOutput : public output::FloatOutput,
                           public AXP192OutputBase
{
    
public:
    void set_min_voltage(uint16_t voltage) { this->min_voltage_ = voltage; }
    void set_max_voltage(uint16_t voltage) { this->max_voltage_ = voltage; }
    void set_channel(PowerChannel channel) { this->channel_ = channel; }
    void set_step(uint16_t step) { this->step_ = step; }

protected:
    void write_state(float state) override;
    
    uint16_t min_voltage_{0};     // mV
    uint16_t max_voltage_{3300};  // mV
    uint16_t step_{100}; // step, refer to axp192_const definations
    PowerChannel channel_;
};

class AXP192BinaryOutput : public output::BinaryOutput,
                            public AXP192OutputBase
{

friend class AXP192FloatOutput;
public:
    void set_voltage(uint16_t voltage) { this->voltage_ = voltage; };
    void set_channel(PowerChannel channel) { this->channel_ = channel; }
protected:
    void write_state(bool state) override;
private:
    uint16_t voltage_{0};
    PowerChannel channel_;
};


} // namespace axp192

} // esphome