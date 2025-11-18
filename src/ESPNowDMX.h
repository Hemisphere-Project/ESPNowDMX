#pragma once

#include <cstddef>

#include "ESPNowDMX_Common.h"
#include "ESPNowDMX_Sender.h"
#include "ESPNowDMX_Receiver.h"

enum ESPNowDMXMode : uint8_t {
	ESPNOW_DMX_MODE_SENDER = 0,
	ESPNOW_DMX_MODE_RECEIVER = 1,
};

class ESPNowDMX {
public:
	ESPNowDMX();

	bool begin(ESPNowDMXMode mode, bool registerInternalEspNow = true);
	void loop();
	void update();

	void setUniverseId(uint8_t universe);
	void setChannel(uint16_t address, uint8_t value);
	void sendDMXFrame(const uint8_t* data, size_t length);

	void setReceiveCallback(DMXReceiveCallback cb);
	void setDMXReceiveCallback(DMXReceiveCallback cb);

	bool handleIncoming(const uint8_t* mac, const uint8_t* data, int len);
	static void forwardPacket(const uint8_t* mac, const uint8_t* data, int len);

private:
	ESPNowDMXMode _mode;
	bool _initialized;
	uint8_t _pendingUniverse;
	ESPNowDMX_Sender _sender;
	ESPNowDMX_Receiver _receiver;

	static ESPNowDMX* s_activeReceiver;
};
