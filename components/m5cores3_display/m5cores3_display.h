#pragma once
#include "esphome/components/display/display_buffer.h"

namespace esphome {
namespace m5cores3_display {

class M5CoreS3Display : public display::Display {
 public:
  void setup() override;
  void dump_config() override;
  int get_width_internal() override;
  int get_height_internal() override;

  display::DisplayType get_display_type() override { return display::DisplayType::DISPLAY_TYPE_COLOR; }
  void fill(Color color) override;
  void draw_pixel_at(int x, int y, Color color) override;
  void update() override;
};

}  // namespace m5cores3_display
}  // namespace esphome
