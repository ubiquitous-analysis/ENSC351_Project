#include <Arduino.h>
#include "ultrasonicSensor.h"
#include "ultrasonicWrapper.h"
ultrasonicSensor s1;
ultrasonicSensor s2;

ultrasonicWrapper w1;

void setup() {
    Serial.begin(115200);
    delay(1000);
    // s1.ultrasonic_initialize(23,22);
    // s2.ultrasonic_initialize(21,19);
    w1.ultrasonicWrapper_initialize(23,22,21,19);
    delay(5000);
    Serial.println("The current callibrated distances are: ");
    Serial.println(w1.ultrasonicWrapper_S1_getCalibratedDistance());
    Serial.println(w1.ultrasonicWrapper_S2_getCalibratedDistance());
    if((w1.ultrasonicWrapper_S1_getCalibratedDistance() == -1) ||(w1.ultrasonicWrapper_S2_getCalibratedDistance() == -1) ){
      delay(1000000000);
    }
}

void loop() {
  //delay(5000);
  // Serial.println("Getting value from ultrasonic 1: ");
  // Serial.println(s1.ultrasonic_getDistance());
  // Serial.println("Getting value from ultrasonic 2: ");
  // Serial.println(s2.ultrasonic_getDistance());


  // Testing ultrasonic Wrapper:
  // Serial.println("Results from getting ultrasonic Wrapper values: ");
  
  Serial.println("Got past the initialization!");
  
  //w1.ultrasonicWrapper_calibrateSensors();
  Serial.println("The current callibrated distances are: ");
  Serial.println(w1.ultrasonicWrapper_S1_getCalibratedDistance());
  Serial.println(w1.ultrasonicWrapper_S2_getCalibratedDistance());
  int readValue = w1.ultrasonicWrapper_readValues();
  Serial.println(readValue);
  if(readValue != 0){
    delay(5000);
  }
  
}

