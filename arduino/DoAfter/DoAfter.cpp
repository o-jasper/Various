//License: public domain, do whatever, all responsibility is yours.
#pragma once

#include "DoAfter.h" //C++ is dumb

DoAfter::DoAfter(int32_t _period, int32_t time)
{ period= _period; allowed_after= time; }

inline char peek_do_now_p(DoAfter* da,int32_t time)
{ return time > da->allowed_after; }

char do_now_p(DoAfter* da, int32_t time, uint8_t max_frac)
{
  if( peek_do_now_p(da,time) )
    { 
      da->allowed_after = time + da->period - 
	                  min(time - da->allowed_after,
			      (max_frac*(int32_t)da->period)/256);
      return 1;
    }
  return 0;
}

char coerce_do_now_p(DoAfter* da, int32_t time)
{ char no_override = peek_do_now_p(da,time);
  da->allowed_after = time + da->period;
  return no_override;
}
