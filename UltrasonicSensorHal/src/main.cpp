#include <Arduino.h>
#include "ultrasonicSensor.h"
ultrasonicSensor s1;
ultrasonicSensor s2;


void setup() {
    Serial.begin(115200);
    delay(1000);
    s1.ultrasonic_initialize(23,22);
    s2.ultrasonic_initialize(21,19);
}

void loop() {
  // put your main code here, to run repeatedly:
  // Serial.println(ultrasonic_getDistance());
  delay(1000);
  Serial.println("Getting value from ultrasonic 1: ");
  Serial.println(s1.ultrasonic_getDistance());
  Serial.println("Getting value from ultrasonic 2: ");
  Serial.println(s2.ultrasonic_getDistance());
}

