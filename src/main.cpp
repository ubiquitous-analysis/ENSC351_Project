#include <array>
#include "network.h"
#include "rfid.h"
#include "breakbeam.h"
#include "hit_detector.h"
#include "ultrasonicWrapper.h"
#include "fob.h"

ultrasonicWrapper w1;

// Creating tasks for FreeRTOS use

void rfidTask(void *parameters) {
	// Scan RFID reader
	unsigned long rfidTag = 0; // output buffer; value not actually used
	while (1) {
		if (rfid_scan(&rfidTag)) {
			network_send_rfid(rfidTag);
		}
		vTaskDelay(pdMS_TO_TICKS(20));
	}
}

void breakbeamTask(void *parameters) {
	// Scan breakbeam sensors
	bool isBroken = false; // output buffer; value not actually used
	while (1) {
		if (breakbeam_scan(&isBroken)) {
			network_send_breakbeam(isBroken);
		}
		vTaskDelay(pdMS_TO_TICKS(10));
	}
}

void hitDetectorTask(void *parameters) {
	// Scan hit detector (accelerometer + onboard ADC)
	bool isHit = false; // output buffer; value not actually used
	while (1) {
		if (hitDetector_scan(&isHit)) {
			network_send_hitDetector(isHit);
		}
		vTaskDelay(pdMS_TO_TICKS(10));
	}
}

void ultrasonicTask(void *parameters) {
	// Scan ultrasonic sensor for distances
	int ultrasonicDistances[2];
	while (1) {
		if (w1.ultrasonicWrapper_readValues(ultrasonicDistances) == 1) {
			network_send_ultrasonic(ultrasonicDistances);
		}
		vTaskDelay(pdMS_TO_TICKS(50));
	}
}

void fobTask(void *parameters) {
	// Scan remote fob for button presses
	enum fobSignal isPressed;
	while (1) {
		if (fob_listen(&isPressed)){
			network_send_fob(isPressed);
		}
		vTaskDelay(pdMS_TO_TICKS(20));
	}
}

void setup() {
	network_init();
	rfid_init();
	breakbeam_init();
	hitDetector_init();
	w1.ultrasonicWrapper_initialize();
	Serial.println("INITIALIZATION COMPLETE!!!");
	fob_init();

	// Initialize background tasks
	// xTaskCreate(funcname, taskname, stacksize, taskparameters, priority, taskhandle)
    xTaskCreate(rfidTask,        "RFID",        3072, NULL, 1, NULL);
    xTaskCreate(breakbeamTask,   "Breakbeam",   2048, NULL, 1, NULL);
    xTaskCreate(hitDetectorTask, "HitDetector", 2048, NULL, 1, NULL);
    xTaskCreate(ultrasonicTask,  "Ultrasonic",  3072, NULL, 1, NULL);
    xTaskCreate(fobTask,         "Fob",         2048, NULL, 1, NULL);
}

void loop() {
    vTaskDelay(pdMS_TO_TICKS(1000));
}
