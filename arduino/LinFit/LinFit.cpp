//License: public domain, do whatever, all responsibility is yours.
#pragma once

#include "LinFit.h" 

void reset(LinFit* lf) //Same as resetting.
{ lf->sum_x=0; lf->sum_x2=0; lf->sum_y=0; lf->sum_xy=0; lf->cnt=0; }

LinFit::LinFit()
{ reset(this); }

//Add a result to the fit.
void add_to_fit(LinFit* lf, int16_t x,int16_t y)
{
  lf->sum_x+= x; lf->sum_x2+= x*x; 
  lf->sum_y+= y; lf->sum_xy+= x*y;
  lf->cnt++;
}
//A way to fight overflow by dividing all the sums.
void divide_down(LinFit* lf, int16_t divisor)
{
//Sneaky trick using `add_to_fit` to add divisor/2 everywhere instead of a result.
  add_to_fit(lf, divisor/2,divisor/2); 
//And divide everything. (Hardly worth reusing `divide_down` of AvFit
  lf->sum_x /= divisor; lf->sum_x2 /= divisor;
  lf->sum_y /= divisor; lf->sum_xy /= divisor;
  lf->cnt /= divisor;
}
//TODO something to detect how high and avoid it somehow?

float f_calc_A(LinFit* lf) //Float version may help in avoiding overflow.
{
  return (lf->cnt*1.0*lf->sum_xy - lf->sum_x*1.0*lf->sum_y)/
          lf->sum_x2*1.0*lf->cnt - lf->sum_x*1.0*lf->sum_x;
}
//Int16_Teger version, factor for if A fractional.
// Basically, _expect_ to overflow!
int32_t l_calc_A(LinFit* lf, int16_t factor) 
{
  return ((lf->cnt*lf->sum_xy - lf->sum_x*lf->sum_y)*factor)/
          lf->sum_x2*lf->cnt - lf->sum_x*lf->sum_x;
}

float f_calc_B(LinFit* lf)
{
  return (-lf->sum_x*1.0*lf->sum_xy - lf->sum_x2*1.0*lf->sum_y)/
          lf->sum_x2*1.0*lf->cnt - lf->sum_x*1.0*lf->sum_x;
}
// Basically, _expect_ to overflow!
int32_t l_calc_B(LinFit* lf)
{
  return (-lf->sum_x*lf->sum_xy - lf->sum_x2*lf->sum_y)/
          lf->sum_x2*lf->cnt - lf->sum_x*lf->sum_x;
}

//TODO chisqr and errors on A and B
