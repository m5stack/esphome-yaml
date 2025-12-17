#pragma once

#include "esphome/components/time/real_time_clock.h"
#include "../powerhub.h"


namespace esphome {
namespace powerhub {
    
class PowerHubTime : public time::RealTimeClock,
                     public Parented<PowerHub>
{

public:
    void update() override { this->read_time(); }
    void write_time();
    void read_time();
};


template<typename... Ts> class WriteAction : public Action<Ts...>, public Parented<PowerHubTime> {
 public:
  void play(Ts... x) override { this->parent_->write_time(); }
};

template<typename... Ts> class ReadAction : public Action<Ts...>, public Parented<PowerHubTime> {
 public:
  void play(Ts... x) override { this->parent_->read_time(); }
};

} // namespace powerhub
} // namespace esphome

