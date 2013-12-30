
#pragma once

#include "cap_sensor.h"

//#ifdef crosscap_capsense_from //Bleh, language limitations. 
inline static int cap_sensor(int nr);
//#endif
//#ifdef crosscap_capdrive_from
inline static int cap_driver(int nr);
//#endif

inline static void crosscap_select_driver(int drive_nr);
inline static void crosscap_deselect_driver(int drive_nr);

//Prepare for using the pins for crosscap.
void crosscap_prepare(int drive_cnt,int sense_cnt);

//Measure the row of one driver. 
// `sums` must have a length of `sense_cnt*sizeof(uint16_t)`
void crosscap_measure_1_drive(uint16_t* sums, int drive_nr,
			      int sense_cnt, int sense_times);

//Measure all the rows.
// `sums` must have a length of `drive_cnt*sense_cnt*sizeof(uint16_t)`
void crosscap_measure(uint16_t* sums, int drive_cnt,int sense_cnt, 
		      int sense_times);

inline static uint16_t crosscap_result(uint16_t* sums, int sense_cnt, 
				       int driver_nr,int sensor_nr);

//Basically resets the result. Do it enough so they dont overflow;
// basically need measure_times*captime_max_time < 2^16
void crosscap_zero_result(uint16_t* sums, int drive_cnt, int sense_cnt);
