
//Capacitive sensing. For averaging see cap-sum.c

//TODO to much documentation in source code?SW

//Note you could use the pullup resistor to pretend there is another value for
// the resistance, but the resistance hooked up is already very high and the
// internal one is probably too low in comparison for useful results.

#pragma once
#include "cap_sensor.h"

//Capacitively senses by using the sense pin to also drive.
// pin--- object_to_sense --- high_resistor --- 5V
//The pin must start in LOW, OUTPUT state (resistance must be high for that!!)
// (analogous of HIGH cannot be done on Atmega, due to the pullup)
int disengaging_capsense(int pin, int max_time)
{ 
  int n;
//Assume initially LOW
#ifdef disengaging_capsense_more_wait //Optionally wait a bit.
    for ( n=0 ; n< disengaging_capsense_more_wait ; n++ ){}
#endif
  noInterrupts(); //No interrupts during the timing process.
  pinMode(pin, INPUT); //Disable yankdown.
  //Wait until it gets pulled up.
    for( n=0 ; n<max_time && digitalRead(pin)==LOW ; n++ ){}
  interrupts(); //Can do interrupts again.
  pinMode(pin, OUTPUT); //Enable yankdown again.
  return n;
}
//Capacitively sense with a drive pin.
// drive_pin--- high_resistor --- object_to_sense (--- ground capacitance)
//                                     |
//                                   sense_pin
//Drive pin must be in OUTPUT mode
//This version assumes the sense pin position is already low.
int drive_capsense_up(int drive_pin, int sense_pin)
{ 
  digitalWrite(drive_pin, HIGH); //Set it to up.
  int i=0; //wait for it to actually be up.
  for( i=0; i< 8192 && digitalRead(sense_pin)!= HIGH ; i++ ){}
  return i;
}
int drive_capsense_down(int drive_pin, int sense_pin)
{ 
  digitalWrite(drive_pin, LOW); //Set it to up.
  int i=0; //wait for it to actually be down.
  for( i=0; i< 8192 && digitalRead(sense_pin)!= LOW ; i++ ){}
  return i;
}
//Gets it low/high. (It is better to not change the direction)
int drive_capsense_ensure(int drive_pin, int which)
{ 
  digitalWrite(drive_pin, which ? HIGH : LOW ); //Set it to low.
  for( int k=0 ; k<1024 ; k++ ){} //Sucky way to wait.
}

//Same as before, but first drives up, then down, right on the edge of
// flipping from one to the other..
//
//TODO the time should be _ridiculously_ short, and i am making it
// dI/dt a lot, maybe i am really inductively sensing?
// Or there is a hysteresis effect?
void edgedrive_capsense(int drive_pin, int sense_pin, int* result)
{ 
  int i=0; 
  if( digitalRead(sense_pin)==LOW ) //Already low, first send it high
    { digitalWrite(drive_pin, HIGH);
      for( i=0; i< 8192 && digitalRead(sense_pin)!=HIGH ; i++ ){}
    }
  //Now should be high, send it to just low.(cant be interrupted here either.)
  noInterrupts(); 
    digitalWrite(drive_pin, LOW);
    for( i=0; i< 8192 && digitalRead(sense_pin)!=LOW ; i++ ){}
  //Timing process.
    digitalWrite(drive_pin, HIGH); //Send it up.	 
    //And time it.
    for( i=0 ; i<1024 && digitalRead(sense_pin)!=HIGH ; i++ ){}
    result[0]= i;
    //Note in principle, we could do some interrupts here.(between timings.)
    digitalWrite(drive_pin, LOW); //Send it back down.
    for( i=0 ; i<1024 && digitalRead(sense_pin)!=LOW ; i++ ){} //And time it.
  interrupts(); //Can do interrupts again.
  result[1]= i;
}
//Change modes.
//Disengaging doesnt need two pins, but if two are used for drive/edge, it can
// use them too.
void disengaging_mode(int sense_pin) 
{
  pinMode(sense_pin, OUTPUT); 
  digitalWrite(sense_pin,LOW);
}
void disengaging_mode_with_drive(int sense_pin, int drive_pin) 
{
  pinMode(drive_pin, OUTPUT); //The drive becomes constantly up
  digitalWrite(drive_pin,HIGH);
  disengaging_mode(sense_pin);
}

void drive_mode(int drive_pin, int sense_pin)
{ 
  pinMode(drive_pin, OUTPUT); 
//Dont really care which way (up/down)
  pinMode(sense_pin, INPUT);
  digitalWrite(sense_pin,LOW); //Must _not_ have the pullup.
}
void edge_mode(int drive_pin,int sense_pin)
{ drive_mode(drive_pin,sense_pin); }
