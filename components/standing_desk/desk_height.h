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

  void set_height_sensor(sensor::Sensor *sensor) { height_sensor_ = sensor; }
  void set_target_height_sensor(sensor::Sensor *sensor) { target_height_sensor_ = sensor; }

 protected:
  sensor::Sensor *height_sensor_{nullptr};
  sensor::Sensor *target_height_sensor_{nullptr};

  void read_actual_height();
  void read_target_height();
};

}  // namespace uart_demo
}  // namespace esphome
