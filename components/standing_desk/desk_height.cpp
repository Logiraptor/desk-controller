#include "desk_height.h"
#include "esphome/core/log.h"

namespace esphome {
namespace uart_demo {

static const char *TAG = "uart_demo";

void UARTDemo::setup() {
}

void UARTDemo::loop() {
  while (this->available()) {
    uint8_t c;
    this->read_byte(&c);

    if (c != 0x99) {
        continue;
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
        ESP_LOGW(TAG, "Checksum failed");
        continue;
    }

    float heightIn = float(heightCm) / 2.54;

    this->the_sensor_->publish_state(heightIn);
  }
}

}  // namespace uart_demo
}  // namespace esphome
