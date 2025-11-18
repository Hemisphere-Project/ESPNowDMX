#include <Arduino.h>
#include "ESPNowDMX_Receiver.h"

ESPNowDMX_Receiver receiver;

void dmxCallback(uint8_t universe, const uint8_t* data) {
  Serial.printf("Received DMX universe %d - first 8 values: %d %d %d %d %d %d %d %d\n",
                universe, data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7]);
}

void onEspNowReceive(const uint8_t *mac, const uint8_t *data, int len) {
  receiver.handleReceive(mac, data, len);
}

void setup() {
  Serial.begin(115200);
  receiver.begin();  // true by default = internal ESP-NOW init
  receiver.setDMXReceiveCallback(dmxCallback);
}

void loop() {
  delay(10);
}
