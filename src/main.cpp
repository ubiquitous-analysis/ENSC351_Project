#include <Arduino.h>
#include "network.h"
#include "rfid.h"

void setup() {
	network_init();
	rfid_init();
}

void loop() {
	unsigned long rfidTag = 0;
	if (rfid_scan(&rfidTag)) {
		network_send_rfid(rfidTag);
	}
}
