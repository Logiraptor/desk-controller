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

    float heighIn = map(value, 5178, 22140, 22.9, 49);

    this->the_sensor_->publish_state(heightIn);
  }
}

void UARTDemo::handle_char_(uint8_t c) {

//   if (c == '\r')
//     return;
//   if (c == '\n') {
//     std::string s(this->rx_message_.begin(), this->rx_message_.end());
//     if (this->the_text_ != nullptr)
//       this->the_text_->publish_state(s);
//     if (this->the_sensor_ != nullptr)
//       this->the_sensor_->publish_state(parse_number<float>(s).value_or(0));
//     if (this->the_binsensor_ != nullptr)
//       this->the_binsensor_->publish_state(s == "ON");
//     this->rx_message_.clear();
//     return;
//   }
}

void UARTDemo::dump_config() {
  LOG_TEXT_SENSOR("", "The Text Sensor", this->the_text_);
  LOG_SENSOR("", "The Sensor", this->the_sensor_);
}

void UARTDemo::write_binary(bool state) {
  this->write_str(ONOFF(state));
}

void UARTDemo::ping() {
  this->write_str("PING");
}

void UARTDemo::write_float(float state) {
  this->write_str(to_string(state).c_str());
}

void UARTDemoBOutput::dump_config() {
  LOG_BINARY_OUTPUT(this);
}

void UARTDemoBOutput::write_state(bool state) {
  this->parent_->write_binary(state);
}

void UARTDemoFOutput::dump_config() {
  LOG_FLOAT_OUTPUT(this);
}

void UARTDemoFOutput::write_state(float state) {
  this->parent_->write_float(state);
}

void UARTDemoSwitch::dump_config() {
  LOG_SWITCH("", "UART Demo Switch", this);
}

void UARTDemoSwitch::write_state(bool state) {
  this->parent_->write_binary(state);
  this->publish_state(state);
}

void UARTDemoButton::dump_config() {
  LOG_BUTTON("", "UART Demo Button", this);
}

void UARTDemoButton::press_action() {
  this->parent_->ping();
}

}  // namespace uart_demo
}  // namespace esphome
