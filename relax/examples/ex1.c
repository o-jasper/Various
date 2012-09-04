// Author: Jasper den Ouden
//  Placed in public domain.
#include "../pre-main.c"

int step_cnt=0;
void step ()
{
  step_cnt++;
// Sources and constraints.
  constraint( field, field_w,
	      mk_fixed_Rect(field_size/field_w, 0.45,0.0, 0.55,0.4),
	      1.0, 0 );
// source( field, field_w, 
//  mk_fixed_Rect(field_size/field_w, 0.48,0.48, 0.52,0.52),
//  -1 ); 
  
  relax_step_2d (field, field_w,field_h);
// Gradually increase size
  if( (step_cnt%32 == 0) && field_w<screen_wid )
    {
      double* old_field= field;
      field= double_field( old_field, field_w,field_h );
      field_w*=2; field_h*=2;
      free(old_field);
      draw();
    }
}

#include "../main.c"
