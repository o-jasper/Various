//License: public domain, do whatever, all responsibility is yours.
#pragma once

#include "AvFit.h"

void reset(PartAvFit* af)
{ af->sum_x=0; af->sum_x2=0; }

void reset(AvFit* af)
{ af->sum_x=0; af->sum_x2=0; af->cnt=0; }

AvFit::AvFit()
{ reset(this); }
PartAvFit::PartAvFit()
{ reset(this); }

void add_to_fit(PartAvFit* af, int16_t x )
{ af->sum_x+= x; af->sum_x2+= x*x; }
void divide_down(PartAvFit* af, int16_t divisor)
{ af->sum_x/= divisor; af->sum_x2/= divisor; }

void add_to_fit(AvFit* af, int16_t x, int16_t w)
{ af->sum_x+= w*x; af->sum_x2+= w*x*x; af->cnt+=w; }

void divide_down(AvFit* af, int16_t divisor)
{ af->sum_x/= divisor; af->sum_x2/= divisor; af->cnt/= divisor; }

float f_calc_av(PartAvFit* af, int32_t cnt)
{ return af->sum_x*1.0/cnt; }

float f_calc_errorsqr(PartAvFit* af, int32_t cnt)
{ return (af->sum_x2 - af->sum_x*af->sum_x*1.0/cnt)*1.0/cnt; }

int32_t l_calc_av(PartAvFit* af, int32_t cnt)
{ return af->sum_x/cnt; }

int32_t l_calc_errorsqr(PartAvFit* af, int32_t cnt)
{ return (af->sum_x2 - af->sum_x*af->sum_x/cnt)/cnt; }


float f_calc_av(AvFit* af)
{ return f_calc_av(af, af->cnt); }

float f_calc_errorsqr(AvFit* af)
{ return f_calc_errorsqr(af, af->cnt); }

int32_t l_calc_av(AvFit* af)
{ return l_calc_av(af, af->cnt); }

int32_t l_calc_errorsqr(AvFit* af)
{ return l_calc_errorsqr(af, af->cnt); }


//TODO get the lib
//int32_t l_calc_error(AvFit* af)
//{ return ((af->sum_x2 - af->sum_x*af->sum_x/af->cnt)/af->cnt); }
