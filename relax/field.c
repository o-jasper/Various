// Author: Jasper den Ouden
//  Placed in public domain.

double* mk_field (int w,int h)
{ double* field= malloc(sizeof(double)*w*h);
  for( int k=0; k < w*h ; k++ )
    { field[k]=0; }
  return field;
}

double* double_field( double* in, int w,int h )
{ double* out= malloc( 4*sizeof(double)*w*h );
  int k=0,j=0;
  for( int j=0 ; j< w*2 ; j++ )
    { for ( int i=0 ; i< h*2 ; i++ )
	{ out[k]= (i==0 || j==0 || i== w*2-1 || j == h*2-1) ? 0 :
	    in[(i/2) + w* (j/2)];
	  k++;
	}
    }
  return out;
}
// SDL drawing.
void set_pixel( unsigned char to, int i,int j, int col)
{ ((unsigned char*)vid->pixels)[3*i+j*vid->pitch+ col]= to; }
// Drawing could be better.. 
void draw_field (double* field, int w,int h, double *min, double* max)
{
  int j=0,k=0; double nmin=+100000000, nmax=-1000000000;
  for( int j=0 ; j < h && j < vid->h ; j++ )
    { for( int i=0 ; i< w && i< vid->w ; i++ )
	{
	  if( field[k] < nmin )
	    { nmin= field[k]; }
	  if( field[k] > nmax )
	    { nmax= field[k]; }
	  // Gray scale.. Can do better. 
	  int intensity= (255* ( field[k] - (*min) ))/((*max)-(*min));
	  set_pixel(intensity, i,j, 0);
	  set_pixel(intensity, i,j, 1);
	  set_pixel(intensity, i,j, 2);
	  k++;
	}
    }
  // Update min/max.
  *min= nmin; *max= nmax;
}

typedef struct
{ int fx;int fy; int tx; int ty; } Rect;

Rect mk_Rect ( int fx, int fy, int tx, int ty )
{ Rect rect; rect.fx=fx; rect.fy=fy; rect.tx=tx; rect.ty=ty; 
  return rect;
}

Rect mk_fixed_Rect ( double gridsize,
		     double fx,double fy, double tx,double ty )
{ return mk_Rect( fx/gridsize, fy/gridsize, tx/gridsize, ty/gridsize ); }

double calc_f (double c,double omega, double grid_size)
{ return omega*omega*grid_size/(c*c*4); }

// f is for wave equations)at one frequency)
//  subsitute phi(omega)cos(omega t) into wave eq
//  f=0 if you don't know what i mean.
void constraint (double* field, int w, Rect r, double to, double f)
{ for( int i= r.fx ; i < r.tx ; i++ )
    { for( int j=r.fy ; j < r.ty ; j++ )
	{ field[i + j*w]= to + f *(i*i + j*j); }
    }
}

/* Note: take into account the grid size in the strength. 
   (actual strength*grid size) */
void source (double* field, int w, Rect r, double strength)
{ for( int i= r.fx; i < r.tx ; i++ )
    { for( int j=r.fy ; j < r.ty ; j++ )
	{ field[i + j*w]-= strength; }
    }
}

void source_fn ( double* field, int w,Rect r,
		 double(*strength_fn)(int,int) )
{ for( int i= r.fx; i < r.tx ; i++ )
    { for( int j=r.fy ; j < r.ty ; j++ )
	{ field[i + j*w]-= strength_fn(i,j); }
    }
}
