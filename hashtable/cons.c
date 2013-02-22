/*
 * cons.c by Jasper den Ouden , Feb 2010, Public domain.
 * No warranty or assurances whatsoever.
 * See README for some more
 *
 * WARNING _it_sucks_ no garbage collection. No -a lot-!
 */

#pragma once

#include "hash4str.c"
#include "symbol.c"

enum
  { Tp_none=5, Tp_done=4,
    Tp_integer=0, Tp_float=1, Tp_cons=2, Tp_symbol=3
  };


void cons_init (uint32_t size)
{
  hash_init(&symbol_hash, size, sizeof(SymEl));
  cur_namespace= mk_Symbol(":",1);
}

void cons_exit()
{
  hash_destr(&symbol_hash);
}

typedef struct Obj
{
  char type;
  union
  {
    int i; double f;
    struct Cons* cons;
    Symbol sym;
  };
} Obj;

typedef struct Cons
{
  Obj car;
  struct Cons* cdr;
} Cons;

Cons* mk_Cons (Obj car, Cons* cdr)
{
  Cons* out= (Cons*)malloc(sizeof(Cons));
  out->car=car; out->cdr=cdr;
  return out;
}

Obj Obj_Cons (Cons* cons)
{
  Obj ret; ret.type= Tp_cons;
  ret.cons= cons;
  return ret;
}
Obj Obj_integer (int i)
{
  Obj ret; ret.type= Tp_integer;
  ret.i= i;
  return ret;
}
Obj Obj_float (double f)
{
  Obj ret; ret.type= Tp_float;
  ret.f= f;
  return ret;
}
Obj Obj_symbol_str (char *str, int strlen)
{
  Obj ret; ret.type= Tp_symbol;
  ret.sym= mk_Symbol(str,strlen);
  return ret;
}

void Cons_exit (Cons* cons)
{
    if( cons!=NULL )
    {   Obj_exit(cons->car);
        Cons_exit(cons->cdr);
        free(cons);
    }
}

void Obj_exit (Obj obj)
{
  switch ( obj.type )
    {
    case Tp_cons:
      Cons_exit(obj.cons); break;
    }
}

char cons_buffer[256];

Obj read_obj (FILE* from);

Cons* read_cons (FILE* from)
{ 
  Cons* ret=NULL;
  while ( 1 )
    {
      Obj obj= read_obj(from);
      if ( obj.type == Tp_done )
	{ return ret; }
      ret= mk_Cons(obj, ret);
    }
  return NULL;
}

/* Reads a single object. (Skips whitespace/comments upto it. */
Obj read_obj (FILE* from)
{
  char last=getc(from);
  while ( last == '\n' || last== ' ' || last== '\t' )
    { last= getc(from);
      if ( last==')' || feof(from) || ferror(from) )
	{ Obj ret; ret.type = Tp_done;
	  return ret;
	}
    }
  if ( last==';' ) /* Rest of line is comment. */
    { while ( last!='\n' )
	{ last=getc(from); }
      return read_obj(from);
    }
  /* Reached end of some sort. */
  if ( last==')' || feof(from) || ferror(from) )
    { Obj ret; ret.type = Tp_done;
      return ret;
    }
  /* Now it _must_ be something. */
  if ( last=='(' ) /* Sub list. */
    { return Obj_Cons( read_cons(from) ); }
  /* Either number or symbol. */
  int i=1, is_float= (cons_buffer[0]=='.');
  cons_buffer[0]=last;
  while ( i<256 && !( last == '\n' || last==' ' || last=='\t'
		      || last== '(' || last== ')') )
    {
      last= getc(from);
      is_float = is_float || (last=='.');
      cons_buffer[i++]=last;
    }
  /* is a symbol. */
  if ( isalpha( cons_buffer[0] ) )
    {  return Obj_symbol_str(cons_buffer, i); }
  /* integer or float. */
  cons_buffer[i]='\0';
  Obj ret;
  if (is_float)
    { ret.type= Tp_float;
      float f;
      sscanf(cons_buffer, "%f", &f);
      ret.f= f;
    }
  else
    { ret.type= Tp_integer;
      sscanf(cons_buffer, "%i", &ret.i);
    }
  return ret;
}

/* Printing. */
void printing_print (char* str, void* ignored)
{ printf(str); } /*Function to print to stdout*/

void Obj_print (Obj obj, void(*print)(char*,void*), void* data)
{
  switch ( obj.type )
    {
    case Tp_integer:
      sprintf(cons_buffer, "%d", obj.i);
      print(cons_buffer,data);
      break;
    case Tp_float:
      sprintf(cons_buffer, "%f", obj.f);
      print(cons_buffer,data);
      break;
    case Tp_cons:
      { Cons* cur= obj.cons;
	print("(",data);
	while (1)
	  {
	    Obj_print(cur->car, print,data);
	    cur = cur->cdr;
	    if ( cur==NULL )
	      { print(")",data);
		break;
	      }
	    else
	      { print(" ",data); }
	  } 
	break;
      }
    case Tp_symbol:
      print(obj.sym.el->str, data);
      break;
    }
}
