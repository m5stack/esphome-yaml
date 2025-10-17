#pragma once

#include "esphome/components/output/float_output.h"
#include "esphome/components/output/binary_output.h"
#include "../axp2101.h"

namespace esphome {
namespace axp2101{

enum PowerChannel {
    DCDC1,
    DCDC2,
    DCDC3,
    DCDC4,
    DCDC5,
    ALDO1,
    ALDO2,
    ALDO3,
    ALDO4,
    BLDO1,
    BLDO2,
    CPUSLDO,
    DLDO1,
    DLDO2,
};

class AXP2101OutputBase : public Parented<AXP2101> {
public:
    void apply_channel(PowerChannel channel, uint16_t voltage);
    void disable_channel(PowerChannel channel);
};

class AXP2101FloatOutput : public output::FloatOutput,
                           public AXP2101OutputBase
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
    uint16_t step_{100}; // step, refer to axp2101_const definations
    PowerChannel channel_;
};

class AXP2101BinaryOutput : public output::BinaryOutput,
                            public AXP2101OutputBase
{

friend class AXP2101FloatOutput;
public:
    void set_voltage(uint16_t voltage) { this->voltage_ = voltage; };
    void set_channel(PowerChannel channel) { this->channel_ = channel; }
protected:
    void write_state(bool state) override;
private:
    uint16_t voltage_{0};
    PowerChannel channel_;
};


} // namespace axp2101

} // esphome