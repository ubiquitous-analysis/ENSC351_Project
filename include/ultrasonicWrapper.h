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
        // initializes both sensors, also calibrates the sensors
        // Calibration can be repeated without errors after initialization
        void ultrasonicWrapper_initialize(void);
        // Uses moving average to find the moving average distance for 1s each
        void ultrasonicWrapper_calibrateSensors();
        // calls sensor cleanup functions
        void ultrasonicWrapper_cleanupSensors();
        // continuously gets values from both sensors for 1s
        // returns 1 on detection, 0 on timeout, -1 on error
        int ultrasonicWrapper_readValues(int *distances); 
        double ultrasonicWrapper_S1_getCalibratedDistance();
        double ultrasonicWrapper_S2_getCalibratedDistance();

    private:
        ultrasonicSensor s1;
        ultrasonicSensor s2;
        double s1_calibrated_distance;
        double s2_calibrated_distance;
};

#endif
