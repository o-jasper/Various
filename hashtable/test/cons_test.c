
#include <stdio.h>
#include <string.h>
#include "../cons.c"

int main( int argc, char* argv[] )
{
    FILE* f = stdout;
    if ( argc == 2 )
    {   f = fopen(argv[1], "r");
        if( f==NULL || ferror(f) )
        { printf("File didnt open or something.\n"); return -1;}
    }
    if ( argc > 2 )
    { printf("Note: anything more then first argument is ignored."); }
  
    cons_init(1024);
    printf( "Initiated.\n" );
    
    printf("File %s opened.\n", argv[1]);
    
    printf("Reading file.\n");
    Obj obj=// read_obj(f); 
        Obj_Cons(read_cons(f));
    printf("Printing next.\n");
    Obj_print(obj, printing_print,NULL);
    
    printf("\nClean up.\n");
    Obj_exit(obj);
    
    printf("\nClosing file, de-initiating.\n");
    if( argc==2 ){ fclose(f); }
    cons_exit();
}
