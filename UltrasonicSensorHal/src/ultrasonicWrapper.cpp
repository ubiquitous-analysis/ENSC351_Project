#include "ultrasonicWrapper.h"
#include <Arduino.h>
#include <cmath>
#define CALIBRATED_COUNT 15
#define LEARNING_CONTANT 0.95 // the fraction of the previous value that is kept when calibrating
#define DETECTION_FRACTION 0.95 // the percentage of the distance that has to be different from the normal sensor reading to detect
#define ERROR_COEFFICIENT 2 // the amount further than the calibrated distance for the sensors to report an error
#define REPEAT_COUNT 2 // the number of times in a row the sensor has to get a reading to register it
#define CALIBRATION_RESOLOUTION 0.1 // 10 to the power of the number of decimal places that are kept when calibrating

void ultrasonicWrapper::ultrasonicWrapper_initialize(const int S1_PIN_MISO, const int S1_PIN_MOSI, const int S2_PIN_MISO, const int S2_PIN_MOSI){
    s1.ultrasonic_initialize(S1_PIN_MISO, S1_PIN_MOSI);
    s2.ultrasonic_initialize(S2_PIN_MISO, S2_PIN_MOSI);
    ultrasonicWrapper_calibrateSensors();
}


void ultrasonicWrapper::ultrasonicWrapper_cleanupSensors(){
    s1.ultrasonic_cleanup();
    s2.ultrasonic_cleanup();
}

void ultrasonicWrapper::ultrasonicWrapper_calibrateSensors(){
    // Test code:
    Serial.println("Starting calibration!!");


    s1_calibrated_distance =  s1.ultrasonic_getDistance();
    s2_calibrated_distance =  s2.ultrasonic_getDistance();

    Serial.println("Collected initial Distances!");
    Serial.println(s1_calibrated_distance);
    Serial.println(s2_calibrated_distance);
    delayMicroseconds(50);

    int s1Count = 0;
    int s2Count = 0;

    long long startingTime = micros(); // start the timer
    while ((s1Count <= CALIBRATED_COUNT && s2Count <= CALIBRATED_COUNT) && ((micros() - startingTime) < 1000000)){
        Serial.println("in the loop!");
        double S1_measuredDistance = s1.ultrasonic_getDistance();
        double S2_measuredDistance = s2.ultrasonic_getDistance();
        Serial.println("Got past getting the distances!");
        Serial.println(S1_measuredDistance,10);
        Serial.println(S2_measuredDistance,10);
        if (S1_measuredDistance == -1 || S2_measuredDistance == -1){continue;} // ignore errors
        // Get the next values and update the calibrated distance
        s1_calibrated_distance =  s1_calibrated_distance * LEARNING_CONTANT + (1-LEARNING_CONTANT)* S1_measuredDistance;
        s2_calibrated_distance =  s2_calibrated_distance * LEARNING_CONTANT + (1-LEARNING_CONTANT)* S2_measuredDistance;
        // Add to the counts in the value doesn't change, otherwise reset them
        if (round(S1_measuredDistance*CALIBRATION_RESOLOUTION) == round(s1_calibrated_distance*CALIBRATION_RESOLOUTION)){ s1Count++;}
        else{s1Count= 0;}
        // Add to the counts in the value doesn't change, otherwise reset them
        if (round(S2_measuredDistance*CALIBRATION_RESOLOUTION) == round(s2_calibrated_distance*CALIBRATION_RESOLOUTION)){ s2Count++;}
        else{s2Count= 0;}
        delayMicroseconds(50);
        Serial.println("Got here!!!");
    }
    Serial.println("Ultrasonic Sensor Calibration Complete!");
    Serial.println("The elapsed time was: ");
    Serial.println(micros() - startingTime);
    return;

}
double ultrasonicWrapper::ultrasonicWrapper_S1_getCalibratedDistance(){
    return s1_calibrated_distance;
}
double ultrasonicWrapper::ultrasonicWrapper_S2_getCalibratedDistance(){
    return s2_calibrated_distance;
}

int ultrasonicWrapper::ultrasonicWrapper_readValues(){
    long long startingTime = micros(); // start the timer
    // initializes count values
    int S1errorCount = 0;
    int S2errorCount = 0;
    int S1hitCount = 0;
    int S2hitCount = 0;

    while ((micros() - startingTime) < 1000000){

        // Get the next values and update the calibrated distance
        double S1_measuredDistance =  s1.ultrasonic_getDistance();
        double S2_measuredDistance =  s2.ultrasonic_getDistance();
        Serial.println("The distances from reading values: ");
        Serial.println(S1_measuredDistance);
        Serial.println(S2_measuredDistance);
        // Use the detected values to set the counts
        if(S1_measuredDistance >= ERROR_COEFFICIENT * s1_calibrated_distance || S1_measuredDistance < 0){
            S1errorCount++;
            S1hitCount = 0;
        }
        else if(S1_measuredDistance <= DETECTION_FRACTION * s1_calibrated_distance){
            S1hitCount++;
            S1errorCount = 0;
        }
        else{
            S1errorCount = 0;
            S1hitCount = 0;
        }
        // same thing for the second sensor.
        if(S2_measuredDistance >= ERROR_COEFFICIENT * s2_calibrated_distance || S2_measuredDistance < 0){
            S2errorCount++;
            S2hitCount = 0;
        }
        else if(S2_measuredDistance <= DETECTION_FRACTION * s2_calibrated_distance){
            S2hitCount++;
            S2errorCount = 0;
        }
        
        else{
            S2errorCount = 0;
            S2hitCount = 0;
        }
        // check to make sure value counts are right, then wait.
        if(S1errorCount >= REPEAT_COUNT || S2errorCount >= REPEAT_COUNT){return -1;};
        if(S1hitCount >= REPEAT_COUNT || S2hitCount >= REPEAT_COUNT){return 1;};
        delayMicroseconds(500);
    }
    return 0;
}
