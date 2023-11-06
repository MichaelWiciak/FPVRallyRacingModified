#ifndef Radio_h
#define Radio_h

#include <FastCRC.h>
#include <limits.h>

struct datapacketstruct {
  uint16_t crc;
  uint16_t unitNum;
  int8_t throttle;
  uint8_t angle;
  uint8_t command;
} __attribute__ ((packed));

void setupRadio(int channel,void (*thisCallBack)(void));
void sendPacket();

extern datapacketstruct incomingRadioPacket,outgoingRadioPacket;

#endif
