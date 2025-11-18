
#pragma once

#include "ESPNowDMX_Common.h"
#include "ESPNowDMX_Utils.h"

class ESPNowDMX_Receiver {
public:
  ESPNowDMX_Receiver();
  bool begin(bool registerInternalEspNow = true);

  void setDMXReceiveCallback(DMXReceiveCallback cb);
  bool handleReceive(const uint8_t *mac, const uint8_t *data, int len);

private:
  uint8_t dmxBuffer[DMX_UNIVERSE_SIZE];
  uint16_t lastSequence;
  DMXReceiveCallback userCallback;
  bool espNowInitialized;

  void processPacket(const uint8_t *data, int len);

  static ESPNowDMX_Receiver* instance;
  static void onDataReceived(const uint8_t *mac, const uint8_t *data, int len);
};
