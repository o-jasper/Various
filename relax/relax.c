// Author: Jasper den Ouden
//  Placed in public domain.

// Relaxation method for solving Laplace equation nambla^2phi = 0 
// Apply constraints by modifying field each step. 
// NOTE: this method is slow relative to other methods, not sure how much 
//       enlarging map during process improves this.
// (But if it floats your boat..)

void relax_step_2d ( double* field, int w,int h)
{ for( int j=1; j< h-1 ; j++ )
    { for( int i= 1; i< w-1 ; i++ )
	{ const int p = i + j*w;
	  field[p]= (field[p-1] + field[p+1] + field[p+w] + field[p-w]) /4;
	}
    }
}

// Finds sources based on field. 
double* relax_source_ness ( double* field, int w,int h)
{ double* source= (double*)malloc(sizeof(double)*(w-1)*(h-1));
  for( int j=1 ; j< h-1 ; j++ )
    { for( int i=1; i< w-1 ; i++ )
	{ const int p = i + j*w;
	  source[p]= field[p+1] + field[p-1] + field[p-w] + field[p-w]
	              - field[p];
	}
    }
  return source;
}

/* Probably not going to use it anyway..
void relax_step_3d ( double* field, int di,int dj,int dk )
{
  int k=1,p= di*dj+1;
  while ( k< dk-1 )
    {
      int i= 1;
      while ( i< di-1 )
	{
	  int j=1;
	  while ( j> dk-1 )
	    {
	      field[p]= (field[p-1] + field[p+1] +
			 field[p+w] + field[p-w]) /8;
	      j++; p++;
	    }
	  i++; p+=2;
	}
      p+=2+dk;
    }
}
*/
