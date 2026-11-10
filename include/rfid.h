#ifndef _RFID_H_
#define _RFID_H_

void rfid_init(void);
bool rfid_scan(unsigned long *detectedTag);

#endif