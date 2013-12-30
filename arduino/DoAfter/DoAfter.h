//License: public domain, do whatever, all responsibility is yours.
#pragma once

#if ARDUINO >= 100
#include "Arduino.h"
#else
#ifndef NO_WProgram
#include "WProgram.h"
#endif
#endif

class DoAfter
{ public:
  DoAfter(int32_t _period, int32_t time=millis());
  int32_t period, allowed_after; 
};

inline char peek_do_now_p(DoAfter* da,int32_t time = millis());

//max_frac is a fraction between 0 and 255 representing max_frac/256 which is
// the maximum fraction that may be substracted after being late.
//NOTE: havent found a good analytical model..
char do_now_p(DoAfter* da, int32_t time= millis(), unsigned char max_frac=64);

char coerce_do_now_p(DoAfter* da,int32_t time=millis());
