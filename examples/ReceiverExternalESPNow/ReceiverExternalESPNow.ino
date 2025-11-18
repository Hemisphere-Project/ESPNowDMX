/*
 * ESPNowDMX - External ESP-NOW Integration Example (Receiver)
 * 
 * This example demonstrates how to integrate ESPNowDMX_Receiver into a project
 * that already uses ESP-NOW for other purposes.
 * 
 * The receiver is initialized with begin(false) to avoid duplicate ESP-NOW init,
 * and you manually route packets to handleReceive() in your own callback.
 * 
 * Use case: You have existing ESP-NOW communication and want to add DMX reception
 */

#include <Arduino.h>
#include "ESPNowDMX_Receiver.h"

// Your custom message structure
struct CustomMessage {
  uint8_t type;
  uint8_t data[32];
};

ESPNowDMX_Receiver receiver;

// DMX data callback
void dmxCallback(uint8_t universe, const uint8_t* data) {
  Serial.printf("DMX universe %d - channels 1-8: %d %d %d %d %d %d %d %d\n",
                universe, data[0], data[1], data[2], data[3], 
                data[4], data[5], data[6], data[7]);
}

// Your ESP-NOW receive callback
void onEspNowReceive(const uint8_t *mac, const uint8_t *data, int len) {
  // Try to handle as DMX packet first
  if (receiver.handleReceive(mac, data, len)) {
    // Was a DMX packet, already processed
    return;
  }
  
  // Not a DMX packet - handle your own messages
  Serial.printf("[Custom] Received %d bytes from %02X:%02X:%02X:%02X:%02X:%02X\n",
                len, mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  
  if (len == sizeof(CustomMessage)) {
    CustomMessage msg;
    memcpy(&msg, data, sizeof(msg));
    Serial.printf("[Custom] Type: %d, Data[0]: %d\n", msg.type, msg.data[0]);
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("\n=== ESPNowDMX External ESP-NOW Integration ===");
  Serial.println("DMX receiver + custom ESP-NOW messages\n");
  
  // Initialize WiFi and ESP-NOW manually
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW init failed!");
    return;
  }
  
  // Register YOUR callback
  esp_now_register_recv_cb(onEspNowReceive);
  
  // Initialize DMX receiver WITHOUT internal ESP-NOW init
  receiver.begin(false);  // false = don't init ESP-NOW internally
  receiver.setDMXReceiveCallback(dmxCallback);
  
  Serial.println("Ready! Listening for DMX and custom messages.");
}

void loop() {
  delay(10);
}
