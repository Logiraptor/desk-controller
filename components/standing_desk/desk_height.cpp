#include "desk_height.h"
#include "esphome/core/log.h"

namespace esphome {
namespace uart_demo {

static const char *TAG = "uart_demo";

float map(float s, float a1, float a2, float b1, float b2)
{
    return b1 + (s-a1)*((b2-b1)/(a2-a1));
}

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

    if (check != 0x01) {
        continue;
    }

    uint8_t msb;
    uint8_t lsb;
    this->read_byte(&lsb);
    this->read_byte(&msb);

    uint16_t value = (msb << 8) | lsb;

    float heightIn = map(value, 5178, 22140, 22.9, 49);

    this->the_sensor_->publish_state(heightIn);
  }
}

}  // namespace uart_demo
}  // namespace esphome
