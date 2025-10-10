// #include "ili9xxx_display.h"
#include "m5cores3_display.h"
#include "esphome/core/application.h"
#include "esphome/core/hal.h"
#include "esphome/core/helpers.h"
#include "esphome/core/log.h"
#include <M5Unified.h>


static const char *const TAG = "m5cores3.display";
static LGFX_Sprite* _canvas  = nullptr;
static bool _is_updated = false;

namespace esphome {
namespace m5cores3_display {

void M5CoreS3Display::setup() {
  ESP_LOGI(TAG, "Setup CoreS3 Display.");

  _canvas = new LGFX_Sprite(&M5.Display);
  _canvas->setPsram(true);
  _canvas->createSprite(M5.Display.width(), M5.Display.height());
}

void M5CoreS3Display::dump_config() {}
int M5CoreS3Display::get_width_internal() { return M5.Display.width(); }
int M5CoreS3Display::get_height_internal() { return M5.Display.height(); }


void M5CoreS3Display::draw_pixel_at(int x, int y, Color color) {
  lgfx::rgb888_t fill_color;
  fill_color.r = color.r;
  fill_color.g = color.g;
  fill_color.b = color.b;
  _canvas->drawPixel(x, y, fill_color);
  _is_updated = true;
}


void M5CoreS3Display::fill(Color color) {
  // ESP_LOGI(TAG, "fill %d", color.raw_32);
  lgfx::rgb888_t fill_color;
  fill_color.r = color.r;
  fill_color.g = color.g;
  fill_color.b = color.b;
  _canvas->fillScreen(fill_color);
  _is_updated = true;
}


void M5CoreS3Display::update() {
  this->do_update_();
  if (_is_updated) {
    _is_updated = false;
    ESP_LOGI(TAG, "push canvas");
    _canvas->pushSprite(0, 0);
  }
}


}
}
