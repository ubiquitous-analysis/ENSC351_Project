#ifndef _ULTRASONIC_H_
#define _ULTRASONIC_H_

#define TIMEOUT_US 30000

class ultrasonicSensor{
    public:
        // Sets initialization flag, sets pins to output mode
        void ultrasonic_initialize(const int PIN_MISO, const int PIN_MOSI);
        // Clears initialization flag, 
        void ultrasonic_cleanup();

        // Returns a distance from the ultrasonic sensor. 
        // There must be at least 50us between calls of this function
        // This function turns off interrupts to get exact timeing
        // returns -1 on timeout (1s)
        // returns -2 when the distance is multiple times the calibrated distance
        double ultrasonic_getDistance();

    private:
        int ECHO_pin;
        int TRIG_pin;
        bool isInitialized = false;
    
        double calculateDistance(long long waitTimeUS);
        void outputPulse( int lengthUS, int pinOut);

};


#endif