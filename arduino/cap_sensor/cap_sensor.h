
#pragma once

#if ARDUINO >= 100 //TODO this might not work, not tested..
#include "Arduino.h"
#else
#ifndef NO_WProgram
#include "WProgram.h"
#endif
#endif

#ifndef captime_max_time 
  #define captime_max_time 64
#endif

int disengaging_capsense(int pin, int max_time= captime_max_time);
int drive_capsense_up(int drive_pin, int sense_pin);
int drive_capsense_down(int drive_pin, int sense_pin);
int drive_capsense_ensure(int drive_pin, int which);
void edgedrive_capsense(int drive_pin, int sense_pin, int* result);

//Changing modes.(disengaging mode can use just one pin, but also both)
void disengaging_mode(int sense_pin); 
void disengaging_mode_with_drive(int sense_pin, int drive_pin);
void drive_mode(int drive_pin, int sense_pin);
void edge_mode(int drive_pin,int sense_pin);
