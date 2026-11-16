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

static const int fobPins[NUM_PINS] = {
    PIN_A, PIN_B, PIN_C, PIN_D, PIN_E_VALID_INPUT
};
static int lastPinState[NUM_PINS] = {
    LOW, LOW, LOW, LOW, LOW
}



struct signalInfo{
    enum gpiod_edge_event_type type;
    int pin;
};
// static function prototype
static enum fobSignal fobReturnValue(struct signalInfo sigInfo);

void fob_init(void){
    printf("Fob - Initializing\n");
    assert(!is_initialized);
    is_initialized = true;
    chip = gpiod_chip_open("/dev/gpiochip2");
    if(chip == NULL){
        perror("Chip not connecting");
    }
    
    settings = gpiod_line_settings_new();
    gpiod_line_settings_set_direction(settings, GPIOD_LINE_DIRECTION_INPUT);    
    gpiod_line_settings_set_edge_detection(settings, GPIOD_LINE_EDGE_BOTH);

    config = gpiod_line_config_new();
    unsigned int offsets[] = {7,8,9,10,11};

    gpiod_line_config_add_line_settings(config, offsets, 5, settings);
    line = gpiod_chip_request_lines(chip, NULL, config);
    // create the buffer for the events
    buffer = gpiod_edge_event_buffer_new(10);
}

void fob_cleanup(void){
    // Free any memory, close files, ...
    printf("Fob - Cleanup\n");
    assert(is_initialized);
    is_initialized = false;
    // Clean up the GPIOD library stuff
    gpiod_line_request_release(line);
    gpiod_edge_event_buffer_free(buffer);
    gpiod_line_config_free(config);
    gpiod_line_settings_free(settings);
    gpiod_chip_close(chip);

}

enum fobSignal fob_listen(){
    int initialCount = gpiod_line_request_wait_edge_events(line, 1000000000LL);

    // update the current state based on the direction of movement
    if(initialCount <= 0 ){
        return NONE;
    }

    int count = gpiod_line_request_read_edge_events(line, buffer, 10);
    for (int i=0;i<count; i++){

        struct gpiod_edge_event *event = gpiod_edge_event_buffer_get_event(buffer, i);
        unsigned int pin = gpiod_edge_event_get_line_offset(event);
        enum gpiod_edge_event_type type = gpiod_edge_event_get_event_type(event);

        enum fobSignal fobSig = fobReturnValue((struct signalInfo){type,pin});
        if(fobSig == NONE){
            continue;
        }
        else{
            return fobSig;
        }
    }
        
    return NONE;
};

static enum fobSignal fobReturnValue(struct signalInfo sigInfo){
    if(sigInfo.type == GPIOD_EDGE_EVENT_FALLING_EDGE){
            return NONE;
        }
        else if(sigInfo.type == GPIOD_EDGE_EVENT_RISING_EDGE){
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
}