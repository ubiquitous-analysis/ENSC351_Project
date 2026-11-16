#include <Arduino.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "fob.h"

#define DEBUG2 false
#define NUM_PINS 5

static const int PIN_A = 4; // GPIO4
static const int PIN_B = 16; // GPIO16
static const int PIN_C = 17; // GPIO17
static const int PIN_D = 18; // GPIO18
static const int PIN_E_VALID_INPUT = 25; // GPIO25

static const int fobPins[NUM_PINS] = { // array of used GPIO pins
    PIN_A, PIN_B, PIN_C, PIN_D, PIN_E_VALID_INPUT
};
static int lastPinState[NUM_PINS] = { // active high
    LOW, LOW, LOW, LOW, LOW
};

// static function prototype
static enum fobSignal fobReturnValue(struct signalInfo sigInfo);

void fob_init(void){
    for (int i = 0; i < NUM_PINS; i++) {
        pinMode(fobPins[i], INPUT_PULLDOWN);
        lastPinState[i] = digitalRead(fobPins[i]);
    }
    delay(100);
    Serial.println("FOB INIT DONE");
}

enum fobSignal fob_listen(){
    for (int i = 0; i < NUM_PINS; i++) { // Scan all pins for rising edge
        int currentState = digitalRead(fobPins[i]);

        if (currentState != lastPinState[i]) {
            enum edgeType type = (currentState == HIGH) ? EDGE_RISING : EDGE_FALLING;
            lastPinState[i] = currentState;

            enum fobSignal fobSig = fobReturnValue((struct signalInfo){type, fobPins[i]});
            if (fobSig != NONE) {
                return fobSig;
            }
        }
    }
    return NONE; // no change? return nothing (fob not transmitting)
}

static enum fobSignal fobReturnValue(struct signalInfo sigInfo){
    if(sigInfo.type == EDGE_FALLING){
            return NONE;
        }
        else if(sigInfo.type == EDGE_RISING){
            switch(sigInfo.pin){
                case PIN_A:
                    return sigA;
                case PIN_B:
                    return sigB;
                case PIN_C:
                    return sigC;
                case PIN_D:
                    return sigD;
                default:
                    return NONE;
        }
    }
    return NONE;
}