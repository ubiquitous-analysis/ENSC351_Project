#ifndef _FOB_H_
#define _FOB_H_

enum fobSignal {
    sigA,
    sigB,
    sigC,
    sigD,
    NONE
};

enum edgeType {
    EDGE_NONE = 0,
    EDGE_RISING,
    EDGE_FALLING
};

struct signalInfo {
    enum edgeType type;
    int pin;
};

// turns on/off pins, sets initialization flag
void fob_init();
void fob_cleanUp();

// Returns one signal on the rising input of that button.
// no debouncing logic needed. Nothing is returned on the falling edge
enum fobSignal fob_listen();

#endif