
#include "ESPNowDMX_Receiver.h"

ESPNowDMX_Receiver* ESPNowDMX_Receiver::instance = nullptr;

ESPNowDMX_Receiver::ESPNowDMX_Receiver()
  : lastSequence(0), userCallback(nullptr), espNowInitialized(false) {
  memset(dmxBuffer, 0, DMX_UNIVERSE_SIZE);
  instance = this;
}

bool ESPNowDMX_Receiver::begin(bool registerInternalEspNow) {
  WiFi.mode(WIFI_STA);
  if (registerInternalEspNow) {
    if (esp_now_init() != ESP_OK) {
      return false;
    }
    esp_now_register_recv_cb(ESPNowDMX_Receiver::onDataReceived);
    espNowInitialized = true;
  }

  esp_now_peer_info_t peer = {};
  memset(peer.peer_addr, 0xFF, 6);
  peer.channel = 0;
  peer.encrypt = false;
  
  esp_err_t result = esp_now_add_peer(&peer);
  if (result != ESP_OK && result != ESP_ERR_ESPNOW_EXIST) {
    return false;
  }

  return true;
}

void ESPNowDMX_Receiver::setDMXReceiveCallback(DMXReceiveCallback cb) {
  userCallback = cb;
}

bool ESPNowDMX_Receiver::handleReceive(const uint8_t *mac, const uint8_t *data, int len) {
  if (len < PACKET_HEADER_SIZE) return false;
  if (data[0] != PACKET_TYPE_DATA_CHUNK) return false;
  processPacket(data, len);
  return true;
}

void ESPNowDMX_Receiver::processPacket(const uint8_t *data, int len) {
  uint8_t universe = data[1];
  uint16_t seq = (data[2] << 8) | data[3];
  uint16_t offset = (data[4] << 8) | data[5];
  const uint8_t *payload = data + PACKET_HEADER_SIZE;
  size_t payloadLen = len - PACKET_HEADER_SIZE;

  // Handle sequence number wrap-around (16-bit counter wraps from 65535 to 0)
  // Accept packet if: new seq > last seq OR seq wrapped around (big backward jump)
  int16_t seqDiff = (int16_t)(seq - lastSequence);
  if (seqDiff <= 0 && lastSequence != 0) return;  // Reject old/duplicate packets
  lastSequence = seq;

  size_t decompressedLen = decompressData(payload, payloadLen, dmxBuffer + offset, DMX_UNIVERSE_SIZE - offset);

  if (userCallback) {
    userCallback(universe, dmxBuffer);
  }
}

void ESPNowDMX_Receiver::onDataReceived(const uint8_t *mac, const uint8_t *data, int len) {
  if (instance) {
    instance->handleReceive(mac, data, len);
  }
}
