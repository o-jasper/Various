
//Uses one set of pins to control which of the capacitive pads is measured 
// with in another set of pins.

#pragma once

#if ARDUINO >= 100 //TODO this might not work, not tested..
#include "Arduino.h"
#else
#ifndef NO_WProgram
#include "WProgram.h"
#endif
#endif

#include "cap_sensor.h"

#ifndef crosscap_sense_from
#define crosscap_sense_from 8
#endif
#ifndef crosscap_drive_from
#define crosscap_drive_from 3
#endif

//#ifdef crosscap_sense_from //Bleh, language limitations. 
inline static int cap_sensor(int nr){ return crosscap_sense_from + nr; }
//#endif
//#ifdef crosscap_drive_from
inline static int cap_driver(int nr){ return crosscap_drive_from + nr; }
//#endif

inline static void crosscap_select_driver(int drive_nr)
{ digitalWrite(cap_driver(drive_nr),HIGH); }
inline static void crosscap_deselect_driver(int drive_nr)
{ digitalWrite(cap_driver(drive_nr),LOW); }

//Prepare for using the pins for crosscap.
void crosscap_prepare(int drive_cnt,int sense_cnt)
{ 
  for( int i=0 ; i<drive_cnt ; i++ )
    { pinMode(cap_driver(i), OUTPUT); //Must be to output.
      crosscap_deselect_driver(i);
    }
  for( int i=0 ; i<sense_cnt ; i++ )
    { disengaging_mode(cap_sensor(i)); }
}

//Measure the row of one driver. 
// `sums` must have a length of `sense_cnt*sizeof(uint16_t)`
void crosscap_measure_1_drive(uint16_t* sums, int drive_nr,
			      int sense_cnt, int sense_times)
{
  crosscap_select_driver(drive_nr);
  for( int n=0 ; n< sense_times ; n++ ) //Run requested number of times.
    { for( int i=0 ; i< sense_cnt ; i++ )
	{ sums[i]+= disengaging_capsense(cap_sensor(i),captime_max_time); }
    }
  crosscap_deselect_driver(drive_nr);
}
//Measure all the rows.
// `sums` must have a length of `drive_cnt*sense_cnt*sizeof(uint16_t)`
void crosscap_measure(uint16_t* sums, int drive_cnt,int sense_cnt, 
		      int sense_times)
{
  for( int j= 0 ; j< drive_cnt ; j++ )
    { crosscap_measure_1_drive(sums +sense_cnt*j, j, sense_cnt,sense_times); }
}

inline static uint16_t crosscap_result(uint16_t* sums, int sense_cnt, 
				       int driver_nr,int sensor_nr)
{ return sums[driver_nr*sense_cnt + sensor_nr]; }

//Basically resets the result. Do it enough so they dont overflow;
// basically need measure_times*captime_max_time < 2^16
void crosscap_zero_result(uint16_t* sums, int drive_cnt, int sense_cnt)
{ for( int k=0 ; k< drive_cnt*sense_cnt ; k++ ){ sums[k]=0; } }
