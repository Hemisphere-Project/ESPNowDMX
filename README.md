# ESPNowDMX

ESPNowDMX is a library for ESP32 devices to transmit and receive DMX lighting control data over ESP-NOW.

## Features

- Separate Sender and Receiver classes
- Single DMX universe (512 channels)
- Adaptive update rate depending on data variation
- Chunked packets with sequence and offset indexing
- Sequence number wrap-around handling (16-bit counter)
- **Heatshrink compression** with automatic raw fallback
- Flexible ESP-NOW integration (standalone or external)
- Receiver callback with full universe DMX data
- Error handling for ESP-NOW operations

## Installation

### Arduino IDE
1. Download this repository as ZIP
2. In Arduino IDE: Sketch → Include Library → Add .ZIP Library
3. Select the downloaded ZIP file

### PlatformIO
Add to your `platformio.ini`:
```ini
lib_deps = 
    https://github.com/Hemisphere-Project/ESPNowDMX.git
```

## Usage

### Standalone Mode (Default)
The library handles ESP-NOW initialization internally:

```cpp
#include "ESPNowDMX_Sender.h"

ESPNowDMX_Sender sender;

void setup() {
    sender.begin();  // Initializes ESP-NOW internally
}
```

### External ESP-NOW Integration
For projects already using ESP-NOW, use `begin(false)`:

```cpp
#include "ESPNowDMX_Receiver.h"

ESPNowDMX_Receiver receiver;

void onEspNowReceive(const uint8_t *mac, const uint8_t *data, int len) {
    // Route DMX packets to receiver
    if (receiver.handleReceive(mac, data, len)) {
        return;  // Was a DMX packet
    }
    // Handle your own custom ESP-NOW messages here
}

void setup() {
    WiFi.mode(WIFI_STA);
    esp_now_init();
    esp_now_register_recv_cb(onEspNowReceive);
    
    receiver.begin(false);  // false = external ESP-NOW management
    receiver.setDMXReceiveCallback(myCallback);
}
```

## API Reference

### ESPNowDMX_Sender

**`bool begin(bool registerInternalEspNow = true)`**
- Initialize the sender
- `registerInternalEspNow`: Set to `false` if managing ESP-NOW externally
- Returns `true` on success

**`void setUniverse(const uint8_t* dmxData)`**
- Update the entire DMX universe buffer (512 bytes)

**`void setChannel(uint16_t address, uint8_t value)`**
- Set a specific DMX channel value
- `address`: DMX channel address (0-511)
- `value`: DMX value (0-255)

**`void loop()`**
- Call frequently in `loop()` to send adaptive updates

### ESPNowDMX_Receiver

**`bool begin(bool registerInternalEspNow = true)`**
- Initialize the receiver
- `registerInternalEspNow`: Set to `false` if managing ESP-NOW externally
- Returns `true` on success

**`void setDMXReceiveCallback(DMXReceiveCallback cb)`**
- Register callback for received DMX data
- Callback signature: `void callback(uint8_t universe, const uint8_t* dmxData)`

**`bool handleReceive(const uint8_t *mac, const uint8_t *data, int len)`**
- Process incoming ESP-NOW packet
- Returns `true` if packet was a DMX packet, `false` otherwise
- Use this when managing ESP-NOW externally

## Examples

### SenderExample
Demonstrates both DMX sending methods:
- **Mode 1** (default): Individual channel control with `setChannel()` - RGB fade
- **Mode 2**: Bulk universe update with `setUniverse()` - channel sweep

Switch modes by editing the `#define` at the top of the sketch.

### ReceiverExample  
Basic DMX receiver that prints received values to serial.

### ReceiverExternalESPNow
Advanced example showing integration with existing ESP-NOW projects.

See `examples/` folder for complete code.

## Technical Details

- **Packet Structure**: Type (1B) + Universe (1B) + Sequence (2B) + Offset (2B) + Compression Flag (1B) + Data
- **Max Chunk Size**: 243 bytes per packet
- **Adaptive Rate**: 33ms when data changes, 100ms when stable
- **Sequence Handling**: Proper 16-bit wrap-around detection
- **Compression**: Heatshrink (LZSS) with automatic raw fallback
  - **Window**: 2^8 (256 bytes)
  - **Lookahead**: 2^4 (16 bytes)
  - **Memory**: ~800 bytes RAM (static allocation)
  - **Best case**: Static scenes compress 40-60%
  - **Worst case**: Random/gradient data sent raw (0% overhead)
  - **Latency**: ~0.5-1ms compression + transmission

## License

GPL-3.0

## Credits
- **ESPNowDMX** by Hemisphere-Project
- **heatshrink** compression library by Scott Vokes / Atomic Object (ISC License)
  - https://github.com/atomicobject/heatshrink
