//Author: Jasper den Ouden
// placed in public domain

#include <stdio.h>
#include <ctype.h>

int main( int argc,const char* argv[] )
{
  FILE* in= stdin;
  if( argc > 1 )
    { in= fopen(argv[0],"r"); };
  if( in==NULL )
    { printf( argc>1 ? "Error opening file.\n" : "Error with stdin.");
      return;
    }
  int error_cnt=0;
  while(!feof(in) && !ferror(in))
    {
      int new_error=0, got=fgetc(in);
      got= toupper(got);
      //putc(got,stdout);
      switch(got)
	{
	case 'T': printf("_"); break;
	case 'M': printf("_ _"); break;
	case 'O': printf("_ _ _"); break;
	case 'H': printf("_ _ _ _"); break;
	  //case 'Ö': printf("___."); break;
	case 'G': printf("_ _."); break;
	case 'Q': printf("_ _._"); break;
	case 'Z': printf("_ _.."); break;
	case 'N': printf("_."); break;
	case 'K': printf("_._"); break;
	case 'Y': printf("_._ _"); break;
	case 'C': printf("_._."); break;
	case 'D': printf("_.."); break;
	case 'X': printf("_.._"); break;
	case 'B': printf("_..."); break;
	case 'E': printf("."); break;
	case 'A': printf("._"); break;
	case 'W': printf("._ _"); break;
	case 'J': printf("._ _ _"); break;
	case 'P': printf("._ _."); break;
	case 'R': printf("._."); break;
	  //case 'Ä': printf("._._"); break;
	case 'L': printf("._.."); break;
	case 'I': printf(".."); break;
	case 'U': printf(".._"); break;
	  //case 'Ü': printf(".._ _"); break;
	case 'F': printf(".._."); break;
	case 'S': printf("..."); break;
	case 'V': printf("..._ "); break;

	case '1': printf("._ _ _ _"); break;
	case '2': printf(".._ _ _"); break;
	case '3': printf("..._ _"); break;
	case '4': printf("...._"); break;
	case '5': printf("....."); break;
	case '6': printf("_...."); break;
	case '7': printf("_ _..."); break;
	case '8': printf("_ _ _.."); break;
	case '9': printf("_ _ _ _."); break;
	case '0': printf("_ _ _ _ _"); break;
	case '.': printf("._._._"); break;
	case ',': printf("_ _.._ _"); break;
	  
	case ' ': case '\t': printf(" "); break;
	case '\n': printf("\n"); break;
	default: new_error=1; error_cnt++; break;
	}
      printf(" ");
      if( error_cnt> 0 && !new_error )
	{ if( error_cnt>1 ){ printf("E%d", error_cnt); }
	  else{ printf("E"); }
	  error_cnt=0;
	}
    }
}
