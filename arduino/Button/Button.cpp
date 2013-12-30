//License: public domain, do whatever, all responsibility is yours.
#pragma once

#include "Button.h"

Button::Button(unsigned char _pin)
{   digitalWrite(_pin,HIGH);
    pinMode(_pin,INPUT);
    pin= _pin;
    state=0;
}

unsigned char pin(Button* button)
{   return button->pin; }

char down(Button* button, char wait)
{ 
    if( digitalRead(button->pin) == LOW )
    { 
        if( button->state < 0 ) //Staying that value.
	{   button->state= -wait; return -3; } 
        button->state--;
        return 1; //up state, but moving.
    }
    if( button->state > 0 ){ button->state= wait; return 3; }//Staying at state.
    button->state++;
    return -1; //Down state, but moving.
}
char downp(Button* button, char wait)
{   return down(button,wait)>0; }
