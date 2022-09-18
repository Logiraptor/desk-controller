#include "esphome.h"

namespace esphome {
namespace timotion {

class DeskHeight : public Component, public UARTDevice, public Sensor {
 public:
  DeskHeight(UARTComponent *parent) : UARTDevice(parent) {}

  char packet_header[3];

  void setup() override {
    // nothing to do here
  }

  void loop() override {

    // Read data from the desk:
    // 0x99 0x40 0x01 a b is a height value
    // Anything else (16 bytes) is a status message

    // Read 3 bytes
    packet_header[0] = packet_header[1];
    packet_header[1] = packet_header[2];
    packet_header[2] = read();

    // Check if it's a height packet
    if (packet_header[0] == 0x99 && 
        packet_header[1] == 0x40 && 
        packet_header[2] == 0x01) {

        packet_header[0] = 0;
        packet_header[1] = 0;
        packet_header[2] = 0;

        // Read 2 bytes
        char packet_height[2];
        packet_height[0] = read();
        packet_height[1] = read();

        // Convert to int with little endian encoding
        int height = (packet_height[1] << 8) | packet_height[0];

        // Publish height
        publish_state(height);
        ESP_LOGD("desk_height", "Received height packet");
    } else {
        // Not a height packet, read the rest of the packet
        for (int i = 0; i < 13; i++) {
          read();
        }
        ESP_LOGD("desk_height", "Received status packet");
    }
  }
};

} // namespace timotion
} // namespace esphome