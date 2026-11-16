#include "network.h"
#include "rfid.h"
#include "breakbeam.h"
#include "hit_detector.h"
#include "ultrasonicWrapper.h"

void setup() {
	network_init();
	rfid_init();
	breakbeam_init();
	hitDetector_init();
	ultrasonicWrapper w1;
	w1.ultrasonicWrapper_initialize()
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

	// Scan hit detector (accelerometer + onboard ADC)
	bool isHit = false; // output buffer; value not actually used
	if (hitDetector_scan(&isHit)) {
		network_send_hitDetector(isHit);
	}

}
