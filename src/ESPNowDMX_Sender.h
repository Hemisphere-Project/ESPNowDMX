
#pragma once

#include "ESPNowDMX_Common.h"
#include "ESPNowDMX_Utils.h"

class ESPNowDMX_Sender {
public:
  ESPNowDMX_Sender();
  bool begin(bool registerInternalEspNow = true);
  void setUniverse(const uint8_t* dmxData);
  void run();

private:
  uint8_t currentUniverse[DMX_UNIVERSE_SIZE];
  uint8_t prevUniverse[DMX_UNIVERSE_SIZE];
  uint16_t seqNumber;
  unsigned long lastSendTime;
  bool espNowInitialized;

  void sendChunk(uint16_t offset, uint16_t length);

  static void onDataSent(const uint8_t *mac_addr, esp_now_send_status_t status);
};
