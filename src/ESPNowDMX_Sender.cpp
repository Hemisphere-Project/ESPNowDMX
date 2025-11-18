
#include "ESPNowDMX_Sender.h"

ESPNowDMX_Sender::ESPNowDMX_Sender()
  : seqNumber(0), lastSendTime(0), espNowInitialized(false) {
  memset(currentUniverse, 0, DMX_UNIVERSE_SIZE);
  memset(prevUniverse, 0, DMX_UNIVERSE_SIZE);
}

bool ESPNowDMX_Sender::begin(bool registerInternalEspNow) {
  WiFi.mode(WIFI_STA);
  if (registerInternalEspNow) {
    if (esp_now_init() != ESP_OK) {
      return false;
    }
    esp_now_register_send_cb(ESPNowDMX_Sender::onDataSent);
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

void ESPNowDMX_Sender::setUniverse(const uint8_t* dmxData) {
  memcpy(currentUniverse, dmxData, DMX_UNIVERSE_SIZE);
}

void ESPNowDMX_Sender::run() {
  unsigned long now = millis();
  const unsigned long rapidInterval = 33;
  const unsigned long slowInterval = 100;

  uint16_t minChanged = DMX_UNIVERSE_SIZE, maxChanged = 0;
  bool anyChange = false;

  for (uint16_t i = 0; i < DMX_UNIVERSE_SIZE; i++) {
    if (currentUniverse[i] != prevUniverse[i]) {
      if (i < minChanged) minChanged = i;
      if (i > maxChanged) maxChanged = i;
      anyChange = true;
    }
  }

  if (!anyChange) {
    if (now - lastSendTime < slowInterval) return;
    minChanged = 0;
    maxChanged = DMX_UNIVERSE_SIZE - 1;
  } else {
    if (now - lastSendTime < rapidInterval) return;
  }

  lastSendTime = now;

  uint16_t chunkLen = maxChanged - minChanged + 1;

  sendChunk(minChanged, chunkLen);

  memcpy(prevUniverse + minChanged, currentUniverse + minChanged, chunkLen);
}

void ESPNowDMX_Sender::sendChunk(uint16_t offset, uint16_t length) {
  uint8_t packet[ESP_NOW_MAX_PAYLOAD];

  packet[0] = PACKET_TYPE_DATA_CHUNK;
  packet[1] = 0;
  packet[2] = (seqNumber >> 8) & 0xFF;
  packet[3] = seqNumber & 0xFF;
  packet[4] = (offset >> 8) & 0xFF;
  packet[5] = offset & 0xFF;

  size_t compressedSize = compressData(currentUniverse + offset, length, packet + PACKET_HEADER_SIZE, ESP_NOW_MAX_PAYLOAD - PACKET_HEADER_SIZE);
  if (compressedSize == 0) {
    compressedSize = length;
    memcpy(packet + PACKET_HEADER_SIZE, currentUniverse + offset, length);
  }

  size_t sendLength = PACKET_HEADER_SIZE + compressedSize;

  esp_now_send(nullptr, packet, sendLength);

  seqNumber++;
}

void ESPNowDMX_Sender::onDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  // Optional callback for send status
}
