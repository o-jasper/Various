//License: public domain, do whatever, all responsibility is yours.
#pragma once

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "AvFit.h"
//TODO AvFit

//Contains all the data gathered for fit.
class LinFit : public AvFit
{ 
 public:
  LinFit();
  int32_t sum_y,sum_xy; //All accumulation vars.
};

void reset(LinFit* lf); //Same thing as init.

void add_to_fit(LinFit* lf, int16_t x,int16_t y);
void divide_down(LinFit* lf, int16_t divisor); //A way to fight overflow.

float f_calc_A(LinFit* lf);
int32_t l_calc_A(LinFit* lf, int16_t factor); //Expect to overflow!

float f_calc_B(LinFit* lf);
int32_t l_calc_B(LinFit* lf); //Expect to overflow!
