//License: public domain, do whatever, all responsibility is yours.
#pragma once

//Averaging with the error over many measurements (using 2 or 3 longs)
// You lose some of the probability distribution information, but at least
// sigma is known.

#if ARDUINO >= 100
#include "Arduino.h"
#else
#ifndef NO_WProgram
#include "WProgram.h"
#endif
#endif

//Doesnt keep count, meant for when you have multiple synchronized avfits.
class PartAvFit 
{
public:
  PartAvFit();
  int32_t sum_x,sum_x2;
};

class AvFit : public PartAvFit
{
public:
  AvFit();
  int32_t cnt;
};

void reset(PartAvFit* af);
void reset(AvFit* af);

void add_to_fit(PartAvFit* af, int16_t x);
void add_to_fit(AvFit* af, int16_t x, int16_t w=1);
void divide_down(PartAvFit* af, int16_t divisor);
void divide_down(AvFit* af, int16_t divisor);

float f_calc_av(PartAvFit* af, int32_t cnt);
float f_calc_errorsqr(PartAvFit* af, int32_t cnt);
int32_t l_calc_av(PartAvFit* af, int32_t cnt);
int32_t l_calc_errorsqr(PartAvFit* af, int32_t cnt);

float f_calc_av(AvFit* af);
float f_calc_errorsqr(AvFit* af);
int32_t l_calc_av(AvFit* af);
int32_t l_calc_errorsqr(AvFit* af);
