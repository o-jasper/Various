//License: public domain, do whatever, all responsibility is yours.
#pragma once

#include "AvFit_write.h"

void print(Stream* ss, PartAvFit* af)
{ ss->print(af->sum_x); ss->print("\t"); ss->print(af->sum_x2); }

void binwrite(Stream* ss, PartAvFit* af)
{ ss->write((uint8_t*)af, sizeof(af)); }

void print(Stream* ss, AvFit* af)
{ print(ss, (PartAvFit*)af);
  ss->print("\t"); ss->print(af->cnt);
}
//Stupidly writes it in binary, will probably be three 'int32' on other side,
// but unsure!
void binwrite(Stream* ss, AvFit* af)
{ ss->write((uint8_t*)af, sizeof(af)); }
