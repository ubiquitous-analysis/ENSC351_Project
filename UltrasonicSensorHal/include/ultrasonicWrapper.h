#ifndef _ULTRASONIC_WRAPPER_H_
#define _ULTRASONIC_WRAPPER_H_
#include "ultrasonicSensor.h"
// Class that does operations to both ultrasonic sensors.
// Operations: 
// - calibrate
// - use the calibration to determine whether there is a dip
// - initialize/cleanup that calls the initialization/cleanup functions of the two classes.
// read, which constantly grabs values until timeout


class ultrasonicWrapper : ultrasonicSensor{
    public: 
        // initializes both sensors
        void ultrasonicWrapper_initialize(const int S1_PIN_MISO, const int S1_PIN_MOSI, const int S2_PIN_MISO, const int S2_PIN_MOSI);
        // one sensor at a time, uses moving average to calibrate distance for 1s each
        void ultrasonicWrapper_calibrateSensors();
        // calls sensor cleanup functions
        void ultrasonicWrapper_cleanupSensors();

        int ultrasonicWrapper_readValues(); // returns 1 on detection, 0 on nothing, -1 on timeout (1s)




    private:
        ultrasonicSensor s1;
        ultrasonicSensor s2;
        double s1_calibrated_distance;
        double s2_calibrated_distance;

};

#endif
