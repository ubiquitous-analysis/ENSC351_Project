

#include "ultrasonicSensor.h"
#include <Arduino.h>
#include <cassert>
#include <cmath>

void ultrasonicSensor::ultrasonic_initialize(const int PIN_MISO, const int PIN_MOSI){
    ECHO_pin = PIN_MISO;
    TRIG_pin = PIN_MOSI;
    pinMode(TRIG_pin, OUTPUT);
    pinMode(ECHO_pin, INPUT);
    digitalWrite(TRIG_pin, LOW);
    delay(50);
    isInitialized = true;
    Serial.println("Ultrasonic  -- Initialized");
}

void ultrasonicSensor::ultrasonic_cleanup(){
  assert(isInitialized);
  isInitialized = false;
  Serial.println("Ultrasonic --Cleanup");
}

double ultrasonicSensor::ultrasonic_getDistance(){
  assert(isInitialized);
  // IMPORTANT - there should not be less than 50us in between the rising edges of the get distance calls.

  outputPulse(50,TRIG_pin);

  // Keep track of the time for timingout
  long long timeoutStartTime = micros();
  // spinlock until the echo reading rising edge
  while(true){
    if(digitalRead(ECHO_pin) == HIGH){
      break;
    }
    else if((timeoutStartTime - micros()) > TIMEOUT_US){
        return -1;
    }
  }
  // get the rising edge from the echo
  long long startingTime = micros();
  while(true){
    if(digitalRead(ECHO_pin) == LOW){
      break;
    }
    else if((micros() - startingTime) > TIMEOUT_US){
        return -1;
    }
  }
  // get the falling edge from echo
  long long waitTime = micros() - startingTime;
  //interrupts(); //Allow interrupts again once values are done.
  return calculateDistance(waitTime);
}

double ultrasonicSensor::calculateDistance(long long waitTimeUS){
    return ((waitTimeUS * 340.0 )/(2.0 * 1000000.0));
}

void ultrasonicSensor::outputPulse( int lengthUS, int pinOut){
    Serial.println("Got to the output pulse");
    digitalWrite(pinOut, HIGH);
    delayMicroseconds(lengthUS);
    digitalWrite(pinOut, LOW);
}
