#ifndef _NETWORK_H_
#define _NETWORK_H_

#include <Arduino.h> // allows use of network_send____ w/o declaring include in every file

void network_init(void);
bool network_send_rfid(unsigned long tag);
bool network_send_breakbeam(bool broken);
bool network_send_hitDetector(bool hit);
bool network_send_ultrasonic(const int distances[2]);
bool network_send_fob(enum fobSignal signal);

#endif