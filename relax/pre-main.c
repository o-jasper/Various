// Author: Jasper den Ouden
//  Placed in public domain.

#include "SDL/SDL.h"
#include <stdio.h>
#include <stdlib.h>

#include <math.h>

SDL_Surface* vid;

#include "relax.c"
#include "field.c"

#define screen_wid 1024
#define screen_height 1024

double field_min,field_max;
int field_w=4,field_h=4; double field_size= 1;
double *field;

void init ()
{ field= mk_field(field_w,field_h); }

void finish()
{ free(field); }

void draw ()
{
  draw_field(field, field_w,field_h, &field_min,&field_max);
  SDL_UpdateRect(vid, 0,0,vid->w,vid->h);
}
