
#pragma once

#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>

// DMX universe size
constexpr uint16_t DMX_UNIVERSE_SIZE = 512;

// Max payload size for ESP-NOW
constexpr uint16_t ESP_NOW_MAX_PAYLOAD = 250;

// Packet types
enum PacketType : uint8_t {
  PACKET_TYPE_DATA_CHUNK = 0x01,
};

// DMX data chunk packet header size
constexpr uint8_t PACKET_HEADER_SIZE = 6; // type(1) + universe(1) + seq(2) + offset(2)

// Max DMX data chunk per packet
constexpr uint16_t MAX_DMX_CHUNK_SIZE = ESP_NOW_MAX_PAYLOAD - PACKET_HEADER_SIZE;

// Type for DMX data buffer
using DMXUniverseBuffer = uint8_t[DMX_UNIVERSE_SIZE];

// User callback type for receiver to receive full buffer
using DMXReceiveCallback = void (*)(uint8_t universe, const uint8_t *dmxData);
