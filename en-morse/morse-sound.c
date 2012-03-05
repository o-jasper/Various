//Author: Jasper den Ouden
// placed in public domain

#include <stdio.h>
#include <ctype.h>

#include "sound.c"

#include "morse-sound-include.c"

//#define _whiny_

int main( int argc,const char* argv[] )
{
  init_morse_sound(argc,argv);
  
  int error_cnt=0;
  while(!feof(in) && !ferror(in))
    {
      int new_error=0, got=fgetc(in);
      switch(got)
	{ case '.': play_sound(0,-1);
	    if( s_delay ){ SDL_Delay(s_delay); } break;
	  case '_': play_sound(1,-1); 
	    if( l_delay ){ SDL_Delay(l_delay); } break;
	  case '\t': case '\n': case ' ': SDL_Delay(letter_delay); break;
	    default: new_error=1; error_cnt++; break;
	}
      if( letter_delay>0 ){ SDL_Delay(letter_delay); }
      if( argc>7 &&  error_cnt> 0 && !new_error )
	{ if( error_cnt>1 ){ printf("E%d", error_cnt); }
	  else{ printf("E"); }
	  error_cnt=0;
	}
    }
  if( error_cnt>0 ){ printf("E"); 
                     if(error_cnt>1){ printf("%d", error_cnt); } }
  exit_sound();
}
