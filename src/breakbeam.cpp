#include <Arduino.h>
#include "breakbeam.h"

// Hardware pins
const int SENSOR_PIN = 27;
const int LED_PIN = 2; // for testing purposes; onboard led on esp32

static int lastReading = -1;

void breakbeam_init(void) {
    // Breakbeam sensor initialization
    pinMode(LED_PIN, OUTPUT);
    pinMode(SENSOR_PIN, INPUT_PULLUP);
    delay(100);
	Serial.println("BREAKBEAM INIT DONE");
}

bool breakbeam_scan(bool *isBroken) {
    // Must be able to handle constant state change as it is running in background
    int reading = digitalRead(SENSOR_PIN);

    if (reading != lastReading) {
        lastReading = reading;
        bool broken = (reading == HIGH); // HIGH = 1 = light beam cut
        Serial.printf("BREAKBEAM STATE CHANGE: %s\n", broken ? "BROKEN" : "UNBROKEN");
        digitalWrite(LED_PIN, broken ? HIGH : LOW); // testing purposes w led
        
        if (isBroken) {
            *isBroken = broken; // save latest state
        }
        return true; // State change occured
    }
    return false; // No state change occured
}

