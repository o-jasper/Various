//Author: Jasper den Ouden
// placed in public domain

#include <stdio.h>
#include <ctype.h>

#include "sound.c"

#include "morse-sound-include.c"

//#define _whiny_

int main( int argc,const char* argv[] )
{
  init_morse_sound(argc,argv); //This guy eats argc and argv as you see.

  int error_cnt=0;
  while(!feof(in) && !ferror(in))
    {
      int new_error=0, got=fgetc(in);
      got= toupper(got);
      //putc(got,stdout);
      switch(got)
	{
	case 'T': morse_sound("_"); break;
	case 'M': morse_sound("__"); break;
	case 'O': morse_sound("___"); break;
	case 'H': morse_sound("____"); break;
	  //case 'Ö': morse_sound("___."); break;
	case 'G': morse_sound("__."); break;
	case 'Q': morse_sound("__._"); break;
	case 'Z': morse_sound("_ _.."); break;
	case 'N': morse_sound("_."); break;
	case 'K': morse_sound("_._"); break;
	case 'Y': morse_sound("_.__"); break;
	case 'C': morse_sound("_._."); break;
	case 'D': morse_sound("_.."); break;
	case 'X': morse_sound("_.._"); break;
	case 'B': morse_sound("_..."); break;
	case 'E': morse_sound("."); break;
	case 'A': morse_sound("._"); break;
	case 'W': morse_sound(".__"); break;
	case 'J': morse_sound(".___"); break;
	case 'P': morse_sound(".__."); break;
	case 'R': morse_sound("._."); break;
	  //case 'Ä': morse_sound("._._"); break;
	case 'L': morse_sound("._.."); break;
	case 'I': morse_sound(".."); break;
	case 'U': morse_sound(".._"); break;
	  //case 'Ü': morse_sound(".._ _"); break;
	case 'F': morse_sound(".._."); break;
	case 'S': morse_sound("..."); break;
	case 'V': morse_sound("..._"); break;

	case '1': morse_sound(".____"); break;
	case '2': morse_sound("..___"); break;
	case '3': morse_sound("...__"); break;
	case '4': morse_sound("...._"); break;
	case '5': morse_sound("....."); break;
	case '6': morse_sound("_...."); break;
	case '7': morse_sound("__..."); break;
	case '8': morse_sound("___.."); break;
	case '9': morse_sound("____."); break;
	case '0': morse_sound("_____"); break;
	case '.': morse_sound("._._._"); break;
	case ',': morse_sound("__..__"); break;
	
	case ' ': case '\t': case '\n': SDL_Delay(l_delay); break;
	  
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
