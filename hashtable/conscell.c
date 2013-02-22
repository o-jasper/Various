
/*
 * conscell.c by Jasper den Ouden , Feb 2010, Public domain.
 * Wrote this quite a while ago now, no warranty or assurances whatsoever.
 * See README for some more
 *
 * WARNING _it_sucks_ no garbage collection.
 */

#pragma once

#include "hash4str.c"

enum
  { Tp_none=-1;
    Tp_integer=0, Tp_float=1, Tp_cons=2, Tp_symbol=3
  };

typedef struct Symbol
{
  hash3str_elem* el;
} Symbol;

typedef struct Obj
{
  char type;
  union
  {
    int i; double f;
    struct ConsCell* cons;
    Symbol sym;
  }
} Obj;

typedef struct Cons
{
  Obj car;
  struct Cons* cdr;
} Cons;

Cons* mk_Cons (Obj car, Cons* cdr)
{
  Cons* out= malloc(sizeof(Cons));
  out->car=car; out->cdr=cdr;
  return out;
}

void Cons_exit (Cons* cons)
{
  Obj_exit(cons->car);
  Cons_exit(cons-cdr);
  free(cons);
}

void Obj_exit (Obj obj)
{
  switch ( obj.type )
    {
    case Tp_cons:
      Cons_exit(obj.cons); break;
    }
}

/* Makes an Cons-list based on the file. */
Cons* read_obj (FILE* from)
{
    //TODO
}

/* Prints the cons-list. */
void printing_print (char* str, void* ignored)
{ printf(str); }

char Obj_print_buffer[256];

void Obj_print (Obj obj, void(*print)(char*,void*), void* data)
{
  switch ( obj.type )
    {
    case Tp_cons:
      { Cons* cur= obj.cons;
	print("(",data);
	while (cur !=NULL)
	  {
	    Obj_print(cur->car, print,data);
	    cur = cur->cdr;
	    print(cur==NULL ? ")":" ",data);
	  }
	break;
      }
    case Tp_integer:
      sprintf(Obj_print_buffer, "%d", obj.i);
      print(Obj_print_buffer,data);
      break;
    case Tp_float:
      sprintf(Obj_print_buffer, "%f", obj.f);
      print(Obj_print_buffer,data);
      break;
    case Tp_symbol:
      print(obj.sym.el->str);
      break;
    }
}}
