#include "ultrasonicWrapper.h"

void ultrasonicWrapper::ultrasonicWrapper_initialize(const int S1_PIN_MISO, const int S1_PIN_MOSI, const int S2_PIN_MISO, const int S2_PIN_MOSI){
    s1.ultrasonic_initialize(S1_PIN_MISO, S1_PIN_MOSI);
    s2.ultrasonic_initialize(S2_PIN_MISO, S2_PIN_MOSI);
}


void ultrasonicWrapper::ultrasonicWrapper_cleanupSensors(){
    s1.ultrasonic_cleanup();
    s2.ultrasonic_cleanup();
}

void ultrasonicWrapper::ultrasonicWrapper_calibrateSensors(){
    
}

