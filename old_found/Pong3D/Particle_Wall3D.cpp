/////////////variables
//
//	timestep		length of time of the simulation step
//
//	nx,ny,nz the normal of the surface, must be length one	//TODO really?
//	ax,ay,az a vector along (paralel to) surface, normalized
//
//	bounce_frict			bouncing friction of surface
//	slide_frict				sliding friction of surface
//	still_frict				friction before it moves	(in reality usually larger)
//	surf_spdx,surf_spdy		speed of the surface along the surface
//	spread					spread of that speed for which still_frict is friction
//
//	spdx,spdy,spdz, speed of the particle
//	accelx,accely,accelz acceleration of the particle
//
//	Tip, let the objects slightly earlier do sliding friction then the bouncing, so that
//	if the objects make tiny bounces they dont go out of the sliding region
//

#ifndef __Particle_Wall3D_cpp_
#define __Particle_Wall3D_cpp_

//#include "J_Standard_defines.h"
#define FLT double

inline bool IsInward_InfiniteWall(const FLT inpr,	const FLT nx,const FLT ny,const FLT nz,
	const FLT px,const FLT py,const FLT pz,	const FLT spdx,const FLT spdy,const FLT spdz)
{	return(nx*spdx+ny*spdy+nz*spdz<0.0 && nx*px+ny*py+nz*pz<inpr);}

//functions below _only_ do colision _response_

//colision without acceleration data kept as data in objects	(changes spd)
void Colide_Bounce(const FLT bounce_frict,const FLT nx,const FLT ny,const FLT nz,
		FLT &spdx,FLT &spdy,FLT &spdz)
{	//bounce
	FLT temp=nx*spdx+ny*spdy+nz*spdz;	//nx,ny,nz must be normalised
	if(temp<0.0)
	{	temp*=(2.0-bounce_frict)/(nx*nx+ny*ny+nz*nz);
		spdx-=temp*nx;
		spdy-=temp*ny;
		spdz-=temp*nz;
	}
}

//colision with acceleration kept as data in objects	(changes accel)
void Colide_Bounce(const FLT bounce_frict,const FLT nx,const FLT ny,const FLT nz,
			const FLT spdx,const FLT spdy,const FLT spdz,FLT &accelx,FLT &accely,FLT &accelz,	const FLT timestep)
{	//bounce
	FLT temp=nx*spdx+ny*spdy+nz*spdz;	//nx,ny,nz must be normalised
	if(temp<0.0)
	{	temp*=(2.0-bounce_frict)/timestep;
		accelx-=temp*nx;
		accely-=temp*ny;
		accelz-=temp*nz;
	}
	
	//stop acceleration
	temp=2*(nx*accelx+ny*accely+nz*accelz);	//nx,ny,nz must be normalised
	if(temp<0.0)
	{	accelx-=temp*nx;
		accely-=temp*ny;
		accelz-=temp*nz;
	}
}

//sliding force, with kept acceleration (MAYBE make one without)
void Colide_Slide(const FLT slide_frict,const FLT still_frict,
	const FLT nx,const FLT ny,const FLT nz,
	const FLT ax,const FLT ay,const FLT az,		const FLT bx,const FLT by,const FLT bz,	
	const FLT surf_spd_a1,const FLT surf_spd_a2,const FLT spread,
	const FLT mass,
	const FLT spdx,const FLT spdy,const FLT spdz,	FLT &accelx,FLT &accely,FLT &accelz,	const FLT timestep)
{	
	if(still_frict!=0.0 ||slide_frict!=0.0)
	{	FLT normalforce=mass*(nx*accelx+ny*accely+nz*accelz);
		if(normalforce<0.0)
		{	
			FLT slide_spd[2];
			slide_spd[0]=ax*spdx+ay*spdy+az*spdz;
			slide_spd[1]=bx*spdx+by*spdy+bz*spdz;	//determine sliding speed along surface in surface coords
					
			slide_spd[0]-=surf_spd_a1;slide_spd[1]-=surf_spd_a2;	//now the relative sliding speed
	
			FLT frict_here,slide_spd_sqr=slide_spd[0]*slide_spd[0]+slide_spd[1]*slide_spd[1];
			if(slide_spd_sqr<spread)	//determine which type of sliding
			{	frict_here=still_frict;	}
			else
			{	frict_here=slide_frict;	}
			
			FLT temp;
			if(1.0<frict_here*frict_here*timestep)	//friction is enough to stop the object this timestep
			{	temp=accelx*ax+accely*ay+accelz*az;
				accelx-=temp*ax;accely-=temp*ay;accelz-=temp*az;
				
				temp=accelx*bx+accely*by+accelz*bz;
				accelx-=temp*bx;accely-=temp*by;accelz-=temp*bz;								
			}
			else	//not enough yet
			{	slide_spd[0]*=frict_here;slide_spd[1]*=frict_here;
				
				accelx-=ax*slide_spd[0];
				accely-=ay*slide_spd[0];	//friction linear with speed, in reality constant
				accelz-=az*slide_spd[0];	//(until it stops and lower on higher speeds i've heard)
				
				accelx-=bx*slide_spd[1];
				accely-=by*slide_spd[1];
				accelz-=bz*slide_spd[1];
			}
		}
	}
}

#endif
