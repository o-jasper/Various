//Author Jasper den Ouden circa 2006
//Placed in public domain

//TODO strangely the ide says not-compiled one way and just works another way

//Snippet code

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <GL/gl.h>
#include "SDL/SDL.h"

#include "SDL/SDL.h"
//#include "SDL/SDL_thread.h"

#include <GL/glu.h>

#include "Pong3D.cpp"

using namespace Pong3D;

Game game;

//static GLint T0 = 0;
//static GLint Frames = 0;

SDL_Event event;
bool done,running;
SDL_Surface *screen;
//SDL_Thread *main_thread=NULL;	//TODO cant get the threading thing to work

int screen_w,screen_h;

//new window size or exposure
void reshape(int &width,int &height)
{
	glClearColor(0.0,0.0,0.0,0.0);
	
	game.Resize(width,height);
	
	glViewport(0,0,(GLint)width, (GLint)height);	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-1.0,1.0,-1.0,1.0,  1.5,20.0);
	glMatrixMode(GL_MODELVIEW);
	
}

void init(int argc, char *argv[])
{	
	glEnable(GL_BLEND);

	game.Init();
	
	glClearColor(0.0,0.0,0.0,0.0);
	
	if (argc > 1 && strcmp(argv[1], "-info")==0)
	{	printf("GL_RENDERER   = %s\n", (char *) glGetString(GL_RENDERER));
		printf("GL_VERSION    = %s\n", (char *) glGetString(GL_VERSION));
		printf("GL_VENDOR     = %s\n", (char *) glGetString(GL_VENDOR));
		printf("GL_EXTENSIONS = %s\n", (char *) glGetString(GL_EXTENSIONS));
	}
	
	SDL_Init(SDL_INIT_VIDEO);
	
		//MAYBE use SDL_NOFRAME and make own frame
	screen = SDL_SetVideoMode(800,600, 16, SDL_OPENGL|SDL_RESIZABLE);
	if(!screen)
	{	fprintf(stderr, "Couldn't set some GL video mode: %s\n", SDL_GetError());
		SDL_Quit();
		exit(2);
	}
	//Enter program name here
	SDL_WM_SetCaption("Pong3D","Pong3D");
	
	reshape(screen->w, screen->h);

	SDL_ShowCursor(0);
}

int main_loop(void *game_data)
{	
	glClear(GL_COLOR_BUFFER_BIT);
	((Game*)game_data)->Draw_State();
	SDL_GL_SwapBuffers();
	
	((Game*)game_data)->Game_Timestep(0.02);
	
	running=false;

};

int main(int argc,char *argv[])
{
	
	init(argc,argv);
	
	while ( !done )
	{	
		while (SDL_PollEvent(&event)) 
		{	switch(event.type)
		    {	case SDL_VIDEORESIZE:
    		   	{	
					screen=SDL_SetVideoMode(event.resize.w, event.resize.h, 16,
					SDL_OPENGL|SDL_RESIZABLE);
					
					if(screen)
	    	   		{	reshape(event.resize.w,event.resize.h);	}
					else 
					{	//Uh oh, we couldn't set the new video mode??
					}
					
					break;
				}
				
				case SDL_QUIT:
				{	done = 1;break;	}
			}
		}
		int x,y;
		SDL_GetMouseState(&x,&y);
 		game.player_f.Mouse_SetCursor(x,y,  screen->w,screen->h);
		
		main_loop((void*)&game);
		
	/*	if(!running && main_thread==NULL)
		{	running=true;
			main_thread=SDL_CreateThread(main_loop,(void*)&game);
		}	*/
		
		SDL_Delay(20);	//50 frames a second is enough TODO lower it if computer is slower then that
		
	//	SDL_WaitThread(main_thread,NULL);

		
	}
	SDL_Quit();
	return(0);             //ANSI C requires main to return int.
}
