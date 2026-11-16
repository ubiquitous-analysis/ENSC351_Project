#ifndef _FOB_H_
#define _FOB_H_

enum fobSignal { // each botton on fob
    sigA,
    sigB,
    sigC,
    sigD,
    NONE
};

enum edgeType { // create custom type to show edges
    EDGE_NONE = 0,
    EDGE_RISING,
    EDGE_FALLING
};

struct signalInfo {
    enum edgeType type;
    int pin;
};

void fob_init();
// Returns one signal on the rising input of that button.
// no debouncing logic needed. Nothing is returned on the falling edge
bool fob_listen(enum fobSignal *isPressed);

#endif