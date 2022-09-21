#include "desk_height.h"
#include "esphome/core/log.h"

namespace esphome {
namespace uart_demo {

static const char *TAG = "uart_demo";

void UARTDemo::read_actual_height() {
  uint8_t c;
  this->read_byte(&c);

  if (c != 0x99) {
      return;
  }

  // 0x40 == not moving
  // 0x41 == moving down
  // 0x42 == moving up
  uint8_t state;
  this->read_byte(&state);

  uint8_t check;
  this->read_byte(&check);

  uint8_t heightCm;
  uint8_t checksum;
  this->read_byte(&heightCm);
  this->read_byte(&checksum);

  if (checksum != ((c + state + check + heightCm) & 0xFF)) {
    ESP_LOGW(TAG, "Checksum failed: %02X %02X %02X %02X %02X", c, state, check, heightCm, checksum);
    return;
  }

  float heightIn = float(heightCm) / 2.54;
  this->height_sensor_->publish_state(heightIn);
}

void UARTDemo::read_target_height() {
  uint8_t buffer[6];
  if (!this->read_array(buffer, 6)) {
    return;
  }

	uint8_t cmd = buffer[0];
	uint8_t unknown = buffer[2];
	uint8_t height = buffer[4];

  if (cmd != buffer[1]) {
    ESP_LOGW(TAG, "Command mismatch: %02X %02X", cmd, buffer[1]);
    return;
  }

  if (unknown != buffer[3]) {
    ESP_LOGW(TAG, "Unknown mismatch: %02X %02X", unknown, buffer[3]);
    return;
  }

  if (height != buffer[5]) {
    ESP_LOGW(TAG, "Height mismatch: %02X %02X", height, buffer[5]);
    return;
  }

  float heightIn = float(height) / 2.54;
  this->target_height_sensor_->publish_state(heightIn);
}

void UARTDemo::send_button_state() {
  ESP_LOGW(TAG, "Sending button state: %02X", this->button_state_);
  uint8_t buffer[5];
  buffer[0] = 0xD8;
  buffer[1] = 0xD8;
  buffer[2] = 0x78;
  buffer[2] = this->button_state_;
  buffer[3] = this->button_state_;
  this->write_array(buffer, 5);
}

void UARTDemo::setup() {
}

void UARTDemo::loop() {
  while (this->available()) {
    uint8_t c;
    this->peek_byte(&c);

    if (c == 0x99) {
      this->read_actual_height();
    } else if (c == 0x98) {
      this->read_target_height();
      this->send_button_state();
    } else {
      this->read_byte(&c);
    }
  }
}

}  // namespace uart_demo
}  // namespace esphome
