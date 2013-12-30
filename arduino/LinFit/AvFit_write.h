//License: public domain, do whatever, all responsibility is yours.
#pragma once

#if ARDUINO >= 100
#include "Arduino.h"
#else
#ifndef NO_WProgram
#include "WProgram.h"
#endif
#endif

#include <Stream.h>
#include "AvFit.h"

void print(Stream* ss, PartAvFit* af);
void binwrite(Stream* ss, PartAvFit* af);

void print(Stream* ss, AvFit* af);
void binwrite(Stream* ss, AvFit* af);
