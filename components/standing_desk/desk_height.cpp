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
    this->handle_char_(c);
  }
}

void UARTDemo::handle_char_(uint8_t c) {

  if (this->rx_message_.size() > 100) {
      ESP_LOGW(TAG, "Message too long, discarding");
      this->rx_message_.clear();
  }

  this->rx_message_.push_back(c);

  if (this->rx_message_.size() < 5)
    return;

  if (this->rx_message_[0] == 0x99 && 
      this->rx_message_[2] == 0x01) {
    ESP_LOGD(TAG, "Found height message");
    uint8_t state = this->rx_message_[1];
    int height = this->rx_message_[3] << 8 | this->rx_message_[4];

    this->rx_message_.erase(this->rx_message_.begin(), this->rx_message_.begin() + 5);

    ESP_LOGD(TAG, "State: %d, Height: %d", state, height);
  } else {
    this->rx_message_.erase(this->rx_message_.begin());
  }
}

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
