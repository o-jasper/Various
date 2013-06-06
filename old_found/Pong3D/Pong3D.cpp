//Author Jasper den Ouden circa 2006
//Placed in public domain

//small game inspired by curveball, a flash game, and my unability to finish 
//other projects

/*Improvement that can be done (but prolly wont be)
	
	Interface:
	-menu, to set AI, some of the variables
	-self made frame (replacing the native)
	-read settings from file
	
	Graphics:
	-without using GL
	-single buffer enough? index mode?
	
	AI:
	-could try make a "evolution simulator"	or AI of this one, only little
	 variables to handle
	
*/


#include "Particle_Wall3D.cpp"

#include "math.h"
namespace Pong3D
{

typedef double Flt;
typedef unsigned Cntr;

class Ball
{	public:
	Ball()
	{	p[0]=0;p[1]=0;p[2]=0;
		spd[0]=0;spd[1]=0;spd[2]=4.0;
		curve[0]=0;curve[1]=0;

		radius=0.05;curve_const=1.0;

		
		ball_detail=12;tail_width=5;
	}
	
	Flt p[3],spd[3];
	Flt curve[2];
	
	Flt curve_const;
	Flt radius;

//Graphics
	short ball_detail;
	short tail_width;
	
	void Ball_Draw();
	void Ball_Draw_Tail(const Flt len=0.4);
};

void Ball::Ball_Draw()
{	
	glBegin(GL_TRIANGLE_FAN);	//TODO maybe line loop anyway?
		glVertex3dv(p);
		
		for(int i=0;i<ball_detail+1;i++)
		{	
			glVertex3d(
				p[0]+radius*cos(2*M_PI*i/ball_detail),
				p[1]+radius*sin(2*M_PI*i/ball_detail),
				p[2]);
				
		}		
	glEnd();
}
void Ball::Ball_Draw_Tail(const Flt len)
{	glLineWidth(tail_width);
	glBegin(GL_LINES);
		glVertex3dv(p);
		glColor3f(0.0,0.0,0.0);
		glVertex3d(p[0]-spd[0]*len,p[1]-spd[1]*len,p[2]-spd[2]*len);
	glEnd();
}

class Racket
{	public:
	Racket()
	{	normal_z=1.0;
		attraction=-700.0;push=0.0;
		p[0]=0.0;p[1]=0.0;p[2]=0.0;
		dim[0]=0.2;dim[1]=0.2;dim[2]=0.1;
		
		curve=4.5;;
		racket_space=-0.1;
		
		red_hit=0.0;red_factor=0.99;red_on_hit=1.0;
		red_size_factor=0.8;red_size_factor2=0.2;
		
		red_width=4;width=2;
	}
	
	Flt normal_z;
	Flt attraction,push;
	Flt p[3],dim[3];
	Flt racket_space;
	
	Flt curve;
	
	void Colide(Ball &ball,  Flt *cursor_p);
	
	void Timestep(const Flt timestep,Flt *cursor_p,Flt *f,Flt *t);

//Graphics
	Flt red_hit,red_factor,red_on_hit;
	Flt red_size_factor,red_size_factor2;
	
	short red_width,width;
	
	void Draw();
};

void Racket::Timestep(const Flt timestep,Flt *cursor_p,Flt *f,Flt *t)
{	
	Flt spd[]={
		attraction*(p[0]-cursor_p[0])*timestep,
		attraction*(p[1]-cursor_p[1])*timestep	};
	
	p[0]+=spd[0]*timestep;p[1]+=spd[1]*timestep;
	
	//may not go out of bounds
	if(p[0]-dim[0]<f[0]+racket_space)
	{	p[0]=f[0]+dim[0]+racket_space;}
	if(p[1]-dim[1]<f[1]+racket_space)
	{	p[1]=f[1]+dim[1]+racket_space;}
	
	if(p[0]+dim[0]>t[0]-racket_space)
	{	p[0]=t[0]-dim[0]-racket_space;}
	if(p[1]+dim[1]>t[1]-racket_space)
	{	p[1]=t[1]-dim[1]-racket_space;}
	
	red_hit*=red_factor;
	if(red_hit<0){red_hit=0;}
}

void Racket::Colide(Ball &ball,  Flt *cursor_p)
{	if(	p[0]-dim[0]-ball.radius<ball.p[0] && ball.p[0]<p[0]+dim[0]+ball.radius  &&
		p[1]-dim[1]-ball.radius<ball.p[1] && ball.p[1]<p[1]+dim[1]+ball.radius  &&
		p[2]-dim[2]-ball.radius<ball.p[2] && ball.p[2]<p[2]+dim[2]+ball.radius
		)
	{	//ok it is inside, do the colide thingy
		Colide_Bounce(-push,   ball.p[0]-p[0],ball.p[1]-p[1],normal_z,  ball.spd[0],ball.spd[1],ball.spd[2]);
		red_hit=red_on_hit;
		
		ball.curve[0]=curve*(p[0]-cursor_p[0]);//TODO has to be a layer before cursor, cursor could be anywhere
		ball.curve[1]=curve*(p[1]-cursor_p[1]);
	}
}

void Racket::Draw()
{	glLineWidth(width);
	glBegin(GL_LINE_LOOP);
		glVertex3f(p[0]+dim[0],p[1]+dim[1],p[2]);
		glVertex3f(p[0]+dim[0],p[1]-dim[1],p[2]);
		glVertex3f(p[0]-dim[0],p[1]-dim[1],p[2]);
		glVertex3f(p[0]-dim[0],p[1]+dim[1],p[2]);
	glEnd();

	glBegin(GL_LINES);
		glVertex3f(p[0]+dim[0],p[1],p[2]);
		glVertex3f(p[0]-dim[0],p[1],p[2]);
		glVertex3f(p[0],p[1]+dim[1],p[2]);
		glVertex3f(p[0],p[1]-dim[1],p[2]);
	glEnd();
	
	//the red thingy
	glLineWidth(red_width);
	if(red_hit>0.01)
	{	glColor3f(red_hit,0.0,0.0);
		glBegin(GL_LINE_LOOP);
			glVertex3f(p[0]+red_size_factor*dim[0],p[1]+red_size_factor*dim[1],p[2]);
			glVertex3f(p[0]+red_size_factor*dim[0],p[1]-red_size_factor*dim[1],p[2]);
			glVertex3f(p[0]-red_size_factor*dim[0],p[1]-red_size_factor*dim[1],p[2]);
			glVertex3f(p[0]-red_size_factor*dim[0],p[1]+red_size_factor*dim[1],p[2]);
		glEnd();
		glBegin(GL_LINE_LOOP);
			glVertex3f(p[0]+red_size_factor2*dim[0],p[1]+red_size_factor2*dim[1],p[2]);
			glVertex3f(p[0]+red_size_factor2*dim[0],p[1]-red_size_factor2*dim[1],p[2]);
			glVertex3f(p[0]-red_size_factor2*dim[0],p[1]-red_size_factor2*dim[1],p[2]);
			glVertex3f(p[0]-red_size_factor2*dim[0],p[1]+red_size_factor2*dim[1],p[2]);
		glEnd();
	}
}

class Player
{	public:
	Player()
	{	agent=0;
		
		wins=0;winlast=false;
		cursor_p[0]=0.0;cursor_p[1]=0.0;
		
		attraction=-1500.0;max_spd_sqr=20.0;
		
		cursor_size=0.05;
		cursor_width=4;
		folower_cursor_size=0.06;
		folower_width=3;folower_diag_width=2;
		
		over_factor=2.0;
		aim_factor=0.95;
	}
	
	unsigned agent;
	
	Cntr wins;
	bool winlast;
	
	Cntr Score(){return(wins);}
	
	Racket racket;
		
	void Timestep(const Flt timestep,  Flt *f,Flt *t);
	
	Flt attraction,max_spd_sqr;
	Flt cursor_p[2],cursor_folow[2];
	//cursor coordinates from the mouse coordinates relative to frame
	inline void Mouse_SetCursor(const int x,const int y,	  const int w,const int h);
	inline void ClampCursor();
	
//AI cursor setting
	void Constant_Predict(Ball &ball)
	{	cursor_p[0]=ball.p[0];cursor_p[1]=ball.p[1];}
	void Linear_Predict(Ball &ball);

	Flt over_factor;
	void Linear_Predict_Over(Ball &ball);
	
	Flt aim_factor;
	void Aimer(Ball &ball,Player& oponent);
	void Aimer_Over(Ball &ball,Player& oponent);
	
	//TODO use of curve? non-linear predict? full predict with walls? learning thingies?
	
//Graphics
	Flt cursor_size;
	short cursor_width;
	Flt folower_cursor_size;
	short folower_width,folower_diag_width;
	
	void Draw_Cursor();
	void Draw_Folower();
};

inline void Player::Mouse_SetCursor(const int x,const int y,	  const int w,const int h)
{	
	cursor_p[0]=1.0-(2.0*(Flt)x)/w;
	cursor_p[1]=-1.0+(2.0*(Flt)(h-y))/h;
}
inline void Player::ClampCursor()
{	if(cursor_folow[0]>1.0){	cursor_folow[0]=1.0;	}
	if(cursor_folow[0]<-1.0){	cursor_folow[0]=-1.0;	}
	
	if(cursor_folow[1]>1.0){	cursor_folow[1]=1.0;	}
	if(cursor_folow[1]<-1.0){	cursor_folow[1]=-1.0;	}
}

void Player::Timestep(const Flt timestep,Flt *f,Flt *t)
{	
	ClampCursor();
	Flt spd[]={
		attraction*(cursor_folow[0]-cursor_p[0])*timestep,
		attraction*(cursor_folow[1]-cursor_p[1])*timestep	};
	
	Flt temp=spd[0]*spd[0]+spd[1]*spd[1];
	if(temp > max_spd_sqr)
	{	temp=sqrt(max_spd_sqr/temp);
		spd[0]*=temp;spd[1]*=temp;
	}
	
	cursor_folow[0]+=spd[0]*timestep;
	cursor_folow[1]+=spd[1]*timestep;
	
	racket.Timestep(timestep,cursor_folow,f,t);
}

void Player::Linear_Predict(Ball &ball)
{	Flt time_to_colide=(racket.p[2]-ball.p[2])/ball.spd[2];
	if(time_to_colide>0)
	{	cursor_p[0]=ball.p[0]+ball.spd[0]*time_to_colide;
		cursor_p[1]=ball.p[1]+ball.spd[1]*time_to_colide;
	}
	else{cursor_p[0]=0.0;cursor_p[1]=0.0;}
}

void Player::Linear_Predict_Over(Ball &ball)
{	Linear_Predict(ball);
	
	cursor_p[0]+=over_factor*(ball.p[0]-racket.p[0]);
	cursor_p[1]+=over_factor*(ball.p[1]-racket.p[1]);
}

void Player::Aimer(Ball &ball,Player& oponent)
{	Linear_Predict(ball);
	if( oponent.racket.p[0]<0.0 )
	{	cursor_p[0]+=aim_factor*racket.dim[0];	}
	else
	{	cursor_p[0]-=aim_factor*racket.dim[0];	}

	if( oponent.racket.p[1]<0.0 )
	{	cursor_p[1]+=aim_factor*racket.dim[1];	}
	else
	{	cursor_p[1]-=aim_factor*racket.dim[1];	}
}

void Player::Aimer_Over(Ball &ball,Player& oponent)
{	Aimer(ball,oponent);

	cursor_p[0]+=over_factor*(ball.p[0]-racket.p[0]);
	cursor_p[1]+=over_factor*(ball.p[1]-racket.p[1]);
}

//Graphics
void Player::Draw_Cursor()
{	
	glLineWidth(cursor_width);
	glBegin(GL_LINES);
		glVertex3d(cursor_p[0],cursor_p[1],racket.p[2]);
		glVertex3d(cursor_p[0]+cursor_size,cursor_p[1]-cursor_size,racket.p[2]);
	glEnd();
}

void Player::Draw_Folower()
{	
	glLineWidth(folower_width);
	glBegin(GL_LINES);
		glVertex3d(cursor_folow[0]+folower_cursor_size,cursor_folow[1]+folower_cursor_size,racket.p[2]);
		glVertex3d(cursor_folow[0]-folower_cursor_size,cursor_folow[1]-folower_cursor_size,racket.p[2]);
		
		glVertex3d(cursor_folow[0]-folower_cursor_size,cursor_folow[1]+folower_cursor_size,racket.p[2]);
		glVertex3d(cursor_folow[0]+folower_cursor_size,cursor_folow[1]-folower_cursor_size,racket.p[2]);
	glEnd();
	glLineWidth(folower_diag_width);
	glBegin(GL_LINES);
		glVertex3d(cursor_folow[0]+0.5*folower_cursor_size,cursor_folow[1],racket.p[2]);
		glVertex3d(cursor_folow[0]-0.5*folower_cursor_size,cursor_folow[1],racket.p[2]);
		
		glVertex3d(cursor_folow[0],cursor_folow[1]+0.5*folower_cursor_size,racket.p[2]);
		glVertex3d(cursor_folow[0],cursor_folow[1]-0.5*folower_cursor_size,racket.p[2]);
	glEnd();
}

class Game	:public Ball
{	public:
	
	void Init()
	{	translate_scene[0]=0.0;translate_scene[1]=0.0;
		scale_scene[0]=1.0;scale_scene[1]=1.0;
	
		f[0]=-0.95;f[1]=-0.95;f[2]=-2.0;
		t[0]=0.95;t[1]=0.95;t[2]=2.0;
		
		player_t.racket.attraction=-200.0;	//ai handycap
		
	//	player_f.racket.normal_z=1.0;
		player_t.racket.normal_z=-1.0;
		player_t.racket.red_width=2;
		
		player_t.racket.p[2]=1.5;
		player_f.racket.p[2]=-1.5;
		
		player_f.agent=0;player_t.agent=5;
		
		wall_normal_z=0.1;
		
		spawn_speed=4.0;
		
		win_wait=700;
		
		view_shift_factor=0.0;view_dist=3.0;	//turned out to be bad idea
		round_width=2;forw_width=3;dist_width=4;
		
		score_width=30;
	}
	
	void Resize(int &new_width,int &new_height);
	unsigned width,height;
	Flt translate_scene[2],scale_scene[2];
	
	Player player_f,player_t;
	
	//playing-box
	Flt f[3],t[3];
	
	//wall normal alteration to stop too large x and y speeds (look vertical and horizontal in game)
	Flt	wall_normal_z;
	
	Flt spawn_speed;
	
	//delay in microseconds when win
	unsigned short win_wait;unsigned spawn_time;

	void Ball_Timestep(const Flt timestep);
	
	void Game_Timestep(const Flt timestep);
	
//Graphical
	Flt view_shift_factor,view_dist;
	short round_width,dist_width,forw_width;

	short score_width;
	
	void Draw_Round_Lines(const Cntr linecnt=10);
	void Draw_Forward_Lines(const Cntr linecnt=5,const Cntr add=0);
	
	void Draw_Ball_Dist_Lines();
	
	void Draw_Score();
	
	void Draw_State();

};

void Game::Resize(int &new_width,int &new_height)
{	
	width=new_width;height=new_height;
	
	if(width<height)	//TODO better cursor behaviure
	{	scale_scene[0]=1.0;
		scale_scene[1]=width/((Flt)height);
	}
	else
	{	scale_scene[0]=height/((Flt)width);
		scale_scene[1]=1.0;
	}
}

void Game::Ball_Timestep(const Flt timestep)
{	
	//wall reflections
	Flt normal_z;
	if(spd[2]>0){	normal_z=wall_normal_z;	}
	else{	normal_z=-wall_normal_z;	}
	if(p[0]<f[0]+radius)
	{	Colide_Bounce(0.0,   1,0,normal_z,  spd[0],spd[1],spd[2]);}
	if(p[1]<f[1]+radius)
	{	Colide_Bounce(0.0,   0,1,normal_z,  spd[0],spd[1],spd[2]);}
	if(p[0]>t[0]-radius)
	{	Colide_Bounce(0.0,   -1,0,normal_z,  spd[0],spd[1],spd[2]);}
	if(p[1]>t[1]-radius)
	{	Colide_Bounce(0.0,   0,-1,normal_z,  spd[0],spd[1],spd[2]);}	
	//win/lose ball
	if(p[2]<f[2]+radius)
	{	player_f.winlast=true;}
	if(p[2]>t[2]-radius)
	{	player_t.winlast=true;}
	
	spd[0]+=curve_const*curve[0]*timestep;
	spd[1]+=curve_const*curve[1]*timestep;
		
	//position step
	p[0]+=spd[0]*timestep;
	p[1]+=spd[1]*timestep;
	p[2]+=spd[2]*timestep;
}

void Game::Game_Timestep(const Flt timestep)
{	
	switch(player_f.agent)
	{	case 0: {break;}	//player
		case 1: player_f.Constant_Predict(this[0]);break;
		
		case 2: player_f.Linear_Predict(this[0]);break;
		case 3:	player_f.Linear_Predict_Over(this[0]);break;
		
		case 4:	player_f.Aimer(this[0],player_t);break;
		case 5:	player_f.Aimer_Over(this[0],player_t);break;
	}

	switch(player_t.agent)
	{	case 0: {break;}	//player
		case 1: player_t.Constant_Predict(this[0]);break;
		
		case 2: player_t.Linear_Predict(this[0]);break;
		case 3:	player_t.Linear_Predict_Over(this[0]);break;
		
		case 4:	player_t.Aimer(this[0],player_f);break;
		case 5:	player_t.Aimer_Over(this[0],player_f);break;
	}	
	
	if( spawn_time < SDL_GetTicks() )
	{	Ball_Timestep(timestep);	}
	
	player_f.Timestep(timestep,  f,t);
	player_t.Timestep(timestep,  f,t);

	player_f.racket.Colide(this[0], player_f.cursor_folow);
	player_t.racket.Colide(this[0], player_t.cursor_folow);
	
	if(player_f.winlast)
	{	player_f.wins++;player_f.winlast=false;
	
		p[0]=0.0;p[1]=0.0;p[2]=1.0;
		spd[0]=0.0;spd[1]=0.0;spd[2]=-spawn_speed;
		
		spawn_time=SDL_GetTicks()+win_wait;
	}
	if(player_t.winlast)
	{	player_t.wins++;player_t.winlast=false;
	
		p[0]=0.0;p[1]=0.0;p[2]=-1.0;
		spd[0]=0.0;spd[1]=0.0;spd[2]=spawn_speed;
		
		spawn_time=SDL_GetTicks()+win_wait;
	}
	
}

//Graphical
void Game::Draw_Round_Lines(const Cntr linecnt)
{	glLineWidth(round_width);
	Flt step=(t[2]-f[2])/linecnt;
	for(Cntr i=1;i<linecnt;i++)
	{	glBegin(GL_LINE_LOOP);
			glVertex3f(f[0],f[1],f[2]+step*i);
			glVertex3f(f[0],t[1],f[2]+step*i);
			glVertex3f(t[0],t[1],f[2]+step*i);
			glVertex3f(t[0],f[1],f[2]+step*i);
		glEnd();
	}
}

void Game::Draw_Forward_Lines(const Cntr linecnt,const Cntr add)
{	
	glLineWidth(forw_width);
	glBegin(GL_LINES);
	
	Flt step=(t[0]-f[0])/(linecnt+add);
	for(Cntr i=1;i<linecnt;i++)
	{	
		glVertex3f(f[0]+step*(i),f[1],f[2]);
		glVertex3f(f[0]+step*(i),f[1],t[2]);
		
		glVertex3f(f[0]+step*(i),t[1],f[2]);
		glVertex3f(f[0]+step*(i),t[1],t[2]);
	}
	step=(t[1]-f[1])/(linecnt+add);
	for(Cntr i=1;i<linecnt;i++)
	{	glVertex3f(f[0],f[1]+step*(i),f[2]);
		glVertex3f(f[0],f[1]+step*(i),t[2]);
		glVertex3f(t[0],f[1]+step*(i),f[2]);
		glVertex3f(t[0],f[1]+step*(i),t[2]);
	}
	
	glEnd();
}

void Game::Draw_Ball_Dist_Lines()
{	glLineWidth(dist_width);
	glBegin(GL_LINE_LOOP);
		glVertex3f(f[0],f[1],p[2]);
		glVertex3f(f[0],t[1],p[2]);
		glVertex3f(t[0],t[1],p[2]);
		glVertex3f(t[0],f[1],p[2]);
	glEnd();

}

void Game::Draw_Score()
{	
	glLineWidth(score_width);
	glBegin(GL_LINES);
		glVertex3d(0.5*(1.0+1/scale_scene[0]),-0.5,player_f.racket.p[2]);
		glVertex3d(0.5*(1.0+1/scale_scene[0]),-0.5+0.05*player_t.wins,player_f.racket.p[2]);
	
	glColor3f(1.0,0.0,0.0);
		glVertex3d(0.5*(-1.0-1/scale_scene[0]),-0.5,player_f.racket.p[2]);
		glVertex3d(0.5*(-1.0-1/scale_scene[0]),-0.5+0.05*player_f.wins,player_f.racket.p[2]);
	glEnd();
}

void Game::Draw_State()
{		
	glColor3f(1.0,1.0,1.0);
	glLoadIdentity();
//	gluLookAt(2.0,2.0,-2.0,	0.0,0.0,0.0,	0.0,1.0,0.0);

	glScalef(scale_scene[0],scale_scene[1],1.0);
	glTranslatef(translate_scene[0],translate_scene[1],0.0);

	gluLookAt(view_shift_factor*player_f.racket.p[0],view_shift_factor*player_f.racket.p[1],-view_dist,
		0.0,0.0,-1.5,	0.0,1.0,0.0);
	
	glColor3f(0.8,0.8,0.8);
	Draw_Round_Lines();
	
	glColor3f(0.0,0.0,1.0);
	Draw_Forward_Lines();
	
	if( spawn_time < SDL_GetTicks() )
	{	glColor3f(0.8,0.0,0.0);
		Draw_Ball_Dist_Lines();
	}
	
	glColor3f(1.0,1.0,1.0);
	player_t.racket.Draw();
	
	if( spawn_time < SDL_GetTicks() )
	{	if(spd[2]>0.0)
		{	glColor3f(0.0,1.0,0.0);
			Ball_Draw();
			glColor3f(1.0,0.0,0.0);
			Ball_Draw_Tail();
		}
		else
		{	glColor3f(1.0,0.0,0.0);
			Ball_Draw_Tail();
			glColor3f(0.0,1.0,0.0);
			Ball_Draw();
		}
	}
	
	glLineWidth(3);
	glColor3f(1.0,1.0,1.0);
	player_f.racket.Draw();
	
	glColor3f(1.0,1.0,1.0);
	player_f.Draw_Folower();
	
	glColor3f(1.0,0.0,0.0);
	player_f.Draw_Cursor();
	
	glColor3f(0.0,0.8,0.1);
	Draw_Score();
}

};	//end namespace Pong3D
