/*
 * symbol.c by Jasper den Ouden , Feb 2010, Public domain.
 * No warranty or assurances whatsoever.
 * See README for some more
 *
 * WARNING _it_sucks_ no garbage collection. No -a lot-!
 */

#pragma once

typedef struct Symbol
{
  hash4str_elem* el;
} Symbol;

typedef struct SymEl
{
  /*  Cons* val; */
/* If this symbol is a package, these are symbols imported from other
   packages. */
  Symbol* defer;int defer_cnt;
} SymEl;

void SymEl_init (SymEl* s)
{
  s->defer_cnt=0;
}

Symbol cur_namespace;
hash4str symbol_hash;

char* Symbol_name (Symbol sym)
{
  if ( sym.el== NULL ) { return ""; }
  char* ret= sym.el->str;
  while ( ret[0]!=':' )
    { ret++; }
  return ret;
}

struct SymEl* Symbol_item (Symbol sym)
{ return (struct SymEl*)hash_elem_item(sym.el); }

/* Makes the symbol, adds namespace(default is current.)
   if not already added.*/
Symbol mk_Symbol (char* str, int strlen)
{ 
  int i=0;
  Symbol ret;
  while ( i< strlen ) /* If namespace manual, just go. */
    { if( str[i++]==':' )
	{ ret.el= *hash_obtain_elem(&symbol_hash, strlen, str);
	  SymEl_init(Symbol_item(ret));
	  return ret;
	}
    }
/* Determine namespace */
  Symbol to_namespace= cur_namespace;
  SymEl* el= Symbol_item( to_namespace );
  i=0;
  while ( i < el->defer_cnt )
    { if( strcmp( str, Symbol_name( el->defer[i] ) ) )
	{ to_namespace= el->defer[i]; break; }
      i++;
    }

  char namespaced_str[512]; //Namespace is encoded using ':' separating.
  const int nlen= to_namespace.el->len;
  char* nstr= to_namespace.el->str;
  i=1;
  while ( i< nlen )
     { namespaced_str[i-1]= nstr[i]; i++; }
  if( nlen<=1 ){ i=0; }
  namespaced_str[i]= ':'; i++;
  int j=0;
  while ( j < strlen )
    { namespaced_str[i++]= str[j++]; }
/* See if in the hash. */
  ret.el= *hash_obtain_elem(&symbol_hash, i, namespaced_str);
  SymEl_init(Symbol_item(ret));
  return ret;
}
