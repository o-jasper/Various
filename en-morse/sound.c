//Author: Jasper den Ouden
// placed in public domain

#include <SDL/SDL_mixer.h>
#include <assert.h>

int sound_cnt=0,sound_max_cnt=-1;
Mix_Chunk** sounds=NULL;

void init_sound (int max_cnt)
{
  if(Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, AUDIO_S16SYS, 2, 4096) != 0) {
    printf("Unable to initialize audio: %s\n", Mix_GetError());
    exit(1);
  }
  sound_max_cnt= max_cnt;
  sounds= (Mix_Chunk**) malloc(sound_max_cnt*sizeof(Mix_Chunk*));
  for( int i=0 ; i< max_cnt ; i++ ){ sounds[i]= NULL; }
}

int load_sound (const char* file)
{
  assert( sound_max_cnt>0 ); //Not initialized yet.
  assert( sound_cnt < sound_max_cnt ); //Too many.
  sounds[sound_cnt++]= Mix_LoadWAV(file);
  return sound_cnt-1;
}

#define max_channel_cnt 256
int channel_cnt=0,channels[max_channel_cnt];

void play_sound (int sound_index, int ticks)
{
  assert( channel_cnt <= max_channel_cnt );

  if( channel_cnt==max_channel_cnt )
    { int i=0; /* Check if all really busy. */
      while( i<channel_cnt )
	{ if( Mix_Playing(channels[i]) )
	    { channels[i] = channels[--channel_cnt]; }
	  else { i++; }
	}
      if( channel_cnt==max_channel_cnt ) /* All really busy. */
	{ printf("Sound maxxed\n");
	  return;
	}
    }
  if( sounds[sound_index]!=NULL )
    { int ch= (ticks==-1) ? 
	        Mix_PlayChannel(-1, sounds[sound_index], 0) :
	        Mix_PlayChannelTimed(-1, sounds[sound_index], 0, ticks);
      int i=0;
      while( i<channel_cnt )
	{ if( channels[i++]==ch ){ break; } }
      if( i==channel_cnt )
	{ channels[channel_cnt++]= ch; }
    }
}

void exit_sound ()
{ int i=0;
  while( i< channel_cnt )
    {  Mix_HaltChannel(channels[i++]); }
  i=0;
  while( i< sound_cnt )
    { Mix_FreeChunk(sounds[i++]); }
  free(sounds);
  Mix_CloseAudio();
}
