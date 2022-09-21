#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/uart/uart.h"

namespace esphome {
namespace uart_demo {

class UARTDemo : public Component,  public uart::UARTDevice {
 public:
  float get_setup_priority() const override { return setup_priority::LATE; }
  void setup() override;
  void loop() override;

  void set_button_state(uint8_t state) { button_state_ = state; }

  float get_desk_height();
  float get_desk_target_height();

  bool is_awake();

 protected:
  float desk_height_{0.0f};
  float desk_target_height_{0.0f};
  uint64_t last_byte_{0};
  uint8_t button_state_{0};

  void read_actual_height();
  void read_target_height();
  void send_button_state();
};

}  // namespace uart_demo
}  // namespace esphome
