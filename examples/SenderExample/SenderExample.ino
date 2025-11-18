#include <Arduino.h>
#include "ESPNowDMX_Sender.h"

// Configuration constants
constexpr uint16_t DMX_START_CHANNEL = 30;
constexpr uint16_t DMX_NUM_CHANNELS = 16;
constexpr uint32_t UPDATE_INTERVAL_MS = 50;

ESPNowDMX_Sender sender;

void setup() {
  Serial.begin(115200);
  sender.begin();

  uint8_t dmxInit[512] = {0};
  sender.setUniverse(dmxInit);
}

void loop() {
  static uint8_t value = 0;
  static unsigned long lastChange = 0;

  if (millis() - lastChange > UPDATE_INTERVAL_MS) {
    lastChange = millis();
    value++;

    uint8_t dmx[512] = {0};
    for (int i = DMX_START_CHANNEL; i < DMX_START_CHANNEL + DMX_NUM_CHANNELS; i++) {
      dmx[i] = value;
    }
    sender.setUniverse(dmx);
  }

  sender.run();
}
