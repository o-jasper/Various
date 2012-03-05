//Author: Jasper den Ouden
// placed in public domain

#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>

int s_delay,l_delay;

void morse_sound(const char* str)
{ 
  for( char* c= str ; *c !='\0' ; c++ )
    { switch(*c)
	{ case '.': play_sound(0,-1);
	    if( s_delay ){ SDL_Delay(s_delay); } break;
	  case '_': play_sound(1,-1); 
	    if( l_delay ){ SDL_Delay(l_delay); } break;
	}
    }
}
int letter_delay= 0;
FILE* in;

int init_morse_sound ( int argc,const char* argv[] )
{
  if( argc > 1 ){ sscanf(argv[1],"%d",&letter_delay); }
 
  in= stdin;
  const int infile_arg_i=2;
  if( argc > infile_arg_i )
    { in= fopen(argv[infile_arg_i],"r"); };
  if( in==NULL )
    { printf( argc>infile_arg_i ? "Error opening file.\n" :
	                          "Error with stdin.");
      return -1;
    }
#ifdef _whiny_
  printf("Init sound.\n");
#endif
  init_sound(3);
#ifdef _whiny_
  printf("Reading sounds.\n");
#endif  
  //TODO read the two files.
  char short_file[256]= "tick.wav"; //TODO default files.
  if( argc > 3 ){ sscanf(argv[3],"%s",short_file); }
  char long_file[256]= "longtick.wav";
  if( argc > 4 ){ sscanf(argv[4],"%s",long_file); }

#ifdef _whiny_
  printf("Actualy..\n"); 
#endif 
  load_sound(short_file); load_sound(long_file);

  if( sounds[0] == NULL )
    { printf("Reading short sound \'%s\' failed.\n", short_file); }
  if( sounds[1] == NULL )
    { printf("Reading long sound \'%s\' failed.\n", long_file); }
  s_delay= sounds[0]->alen/24;//MIX_DEFAULT_FREQUENCY;
  l_delay= sounds[1]->alen/24;//MIX_DEFAULT_FREQUENCY;
  if( argc > 5 ){ sscanf(argv[5],"%d",&s_delay); }
  if( argc > 6 ){ sscanf(argv[6],"%d",&l_delay); }
  
#ifdef _whiny_
  printf("Got delays %d, %d (%d,%d)\n", s_delay,l_delay, 
	 sounds[0]->alen,sounds[1]->alen);
  printf("Starting..\n"); 
#endif  
}
