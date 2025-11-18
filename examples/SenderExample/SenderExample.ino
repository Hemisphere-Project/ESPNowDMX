#include <Arduino.h>
#include "ESPNowDMX_Sender.h"

// Choose example mode (uncomment one):
#define MODE_SETCHANNEL    // Individual channel control (RGB fade)
// #define MODE_SETUNIVERSE   // Bulk universe update (channel sweep)

#ifdef MODE_SETCHANNEL
// Individual channel mode - RGB fade example
constexpr uint16_t DMX_CHANNEL_RED = 1;
constexpr uint16_t DMX_CHANNEL_GREEN = 2;
constexpr uint16_t DMX_CHANNEL_BLUE = 3;
constexpr uint32_t UPDATE_INTERVAL_MS = 20;
#else
// Universe mode - channel sweep example
constexpr uint16_t DMX_START_CHANNEL = 30;
constexpr uint16_t DMX_NUM_CHANNELS = 16;
constexpr uint32_t UPDATE_INTERVAL_MS = 50;
#endif

ESPNowDMX_Sender sender;

void setup() {
  Serial.begin(115200);
  sender.begin();

#ifdef MODE_SETCHANNEL
  Serial.println("ESPNowDMX Sender - Individual Channel Control (RGB Fade)");
#else
  Serial.println("ESPNowDMX Sender - Universe Update (Channel Sweep)");
  uint8_t dmxInit[512] = {0};
  sender.setUniverse(dmxInit);
#endif
}

void loop() {
  static unsigned long lastUpdate = 0;

  if (millis() - lastUpdate > UPDATE_INTERVAL_MS) {
    lastUpdate = millis();

#ifdef MODE_SETCHANNEL
    // Example 1: Individual channel control
    // Create smooth RGB fade using setChannel()
    static float phase = 0;
    phase += 0.05;
    
    uint8_t red = (sin(phase) + 1.0) * 127.5;
    uint8_t green = (sin(phase + 2.094) + 1.0) * 127.5;  // 120° phase shift
    uint8_t blue = (sin(phase + 4.189) + 1.0) * 127.5;   // 240° phase shift
    
    sender.setChannel(DMX_CHANNEL_RED, red);
    sender.setChannel(DMX_CHANNEL_GREEN, green);
    sender.setChannel(DMX_CHANNEL_BLUE, blue);
#else
    // Example 2: Bulk universe update
    // Cycle values across multiple channels using setUniverse()
    static uint8_t value = 0;
    value++;

    uint8_t dmx[512] = {0};
    for (int i = DMX_START_CHANNEL; i < DMX_START_CHANNEL + DMX_NUM_CHANNELS; i++) {
      dmx[i] = value;
    }
    sender.setUniverse(dmx);
#endif
  }

  // Must call loop() regularly to send updates
  sender.loop();
}
