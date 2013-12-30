//License: public domain, do whatever, all responsibility is yours.
#pragma once
#if ARDUINO >= 100
#include "Arduino.h"
#else
#ifndef NO_WProgram
#include "WProgram.h"
#endif
#endif

class Button
{
public:
  Button(unsigned char _pin);
  unsigned char pin; 
  char state;
};

unsigned char pin(Button* button);
char  down(Button* button, char wait=126); //-2 went down, -1 down, +1 up, +2 went up.

char downp(Button* button, char wait=126); //0 if considered down, 1 if considered up.
