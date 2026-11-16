#include "network.h"
#include "rfid.h"
#include "breakbeam.h"

void setup() {
	network_init();
	rfid_init();
	breakbeam_init();
}

void loop() {
	// Scan RFID reader
	unsigned long rfidTag = 0; // output buffer; value not actually used
	if (rfid_scan(&rfidTag)) {
		network_send_rfid(rfidTag);
	}

	// Scan breakbeam sensors
	bool isBroken = false; // output buffer; value not actually used
	if (breakbeam_scan(&isBroken)) {
		network_send_breakbeam(isBroken);
	}
}
