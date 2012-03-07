// Author: Jasper den Ouden
//  Placed in public domain.

#include "SDL/SDL.h"
#include <stdio.h>
#include <stdlib.h>

#include <math.h>

void quit ()
{ 
  printf("Quit requested, quitting.\n");
  exit(0);
}

int main(int argc, char *argv[])
{
  if ( SDL_Init(SDL_INIT_VIDEO) < 0 ) {
    fprintf(stderr, "Couldn't initialize SDL: %s\n", SDL_GetError());
    exit(1);
  }
  atexit(finish);
  SDL_EventState(SDL_KEYUP, SDL_IGNORE);
  
  if ( SDL_SetVideoMode(screen_wid,screen_height, 24, 0) == NULL ) {
    fprintf(stderr, "Couldn't set 1024x1024 video mode: %s\n",
	    SDL_GetError());
    exit(1);
  }
  vid= SDL_GetVideoSurface();
  
  init();
  
  int continuous_draw=0, continuous_step=1;
  SDL_Event event;
  while ( 1 )
    {
    while ( SDL_PollEvent(&event) )
      {      
      switch (event.type)
	{
	case SDL_ACTIVEEVENT:
	  {
	    if ( event.active.state & SDL_APPACTIVE ) {
	      if ( event.active.gain ) {
		printf("App activated\n");
	      } else {
		printf("App iconified\n");
	      }
	    }
	    break;
	  }
	case SDL_KEYDOWN:
	  {
	    switch ( event.key.keysym.sym )
	      { 
	      case SDLK_d: 
		if ( !continuous_draw ){ draw(); } 
		printf ("step %d\nw %d h %d min %f max %f\n", 
			step_cnt, field_w,field_h,field_min,field_max);
		break;
	      case SDLK_f:
		continuous_draw= !continuous_draw; break;
	      case SDLK_z:
		step();draw(); break;
	      case SDLK_x:
		continuous_step= !continuous_step; break;
	      case SDLK_ESCAPE: case SDLK_q:
		quit(); break;
	      }
	    break;
	  }
	case SDL_QUIT:
	  quit();
	  break;
	}
      }
    if( continuous_draw ){ draw(); }
      if( continuous_step ){ step(); }
    }
  // This should never happen.
  printf("SDL_WaitEvent error: %s\n", SDL_GetError());
  exit(1);
}

