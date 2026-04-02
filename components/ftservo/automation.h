#pragma once

#include "esphome/core/automation.h"
#include "ftservo.h"


namespace esphome {
namespace ftservo {

template<typename... Ts> 
class MoveAction : public Action<Ts...>, 
                   public Parented<FTServo> 
{
  TEMPLATABLE_VALUE(int, position)
  TEMPLATABLE_VALUE(int, time)
  TEMPLATABLE_VALUE(int, speed)
 public:
  void play(const Ts &...x) override { this->parent_->move(
    this->position_.value(x...),
    this->time_.value(x...),
    this->speed_.value(x...)); 
  }
};


class MoveTrigger : public Trigger<> {
  
  public:
    explicit MoveTrigger(FTServo *servo) {
      servo->add_move_callback([this]() {
        this->trigger();
      });
    }
};

class ErrorTrigger : public Trigger<const LogString *> {
 public:
  explicit ErrorTrigger(FTServo *servo) {
    servo->add_error_callback([this](const LogString *err) {
      this->trigger(err);
    });
  }
};


template<typename... Ts> class IsMovingCondition : public Condition<Ts...>, public Parented<FTServo> {
 public:
  bool check(const Ts &...x) override { return this->parent_->is_moving(); }
};


}
}

