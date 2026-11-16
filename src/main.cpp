#include "network.h"
#include "rfid.h"
#include "breakbeam.h"
#include "accelerometer.h"
#include "adc.h"
#include "hit_detector.h"

void setup() {
	network_init();
	rfid_init();
	breakbeam_init();
	hitDetector_init();
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

	// Scan accelerometer
	hitDetector_update();
	if (hitDetector_hit()

	// Scan accelerometer for impact

}
