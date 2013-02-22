/*
 * hash4str.c by Jasper den Ouden , Feb 2010, Public domain.
 * Wrote this quite a while ago now, no warranty or assurances whatsoever.
 * See README for some more
 */

#pragma once

#include <string.h>     /* For strcmp. */

#include "lookup3.c"	/* For the keys. */

typedef struct hash4str_elem
{
/*Enable it for listage.*/
	struct hash4str_elem *next;
	
/*First and second key. (second to speed up unequality check)*/
	uint32_t key1,key2;
/*The string*/
	unsigned char len; char* str;
/* If you want to associate some data, set extra_alloc to how much in bytes.
 * Get a pointer to this data, use hash_elem_item
 */
} hash4str_elem;

void* hash_elem_item( hash4str_elem* e )
{	return ((char*)e)+sizeof(hash4str_elem); }

typedef struct hash4str
{	int extra_alloc, cnt;
	
	uint32_t mask, size;
	
	hash4str_elem** elem;
} hash4str;

hash4str_elem* hash_make_elem(const char len, const char *str,
		const uint32_t key1,const uint32_t key2, const int extra_alloc)
{	hash4str_elem* t= 
           (hash4str_elem*)malloc( sizeof(hash4str_elem)+extra_alloc );
	t->next=NULL;
	t->key1=key1; t->key2=key2;

	t->len=len; t->str=(char*)malloc(len+1);
	memcpy(t->str, str, len); 
	t->str[len]='\0';

	return t;
}

/* Raw version of hash_get_elem, you can supply the keys here */
hash4str_elem** hash_get_elem_raw(hash4str* h, 
                                         const unsigned char len, const char* str,
		const uint32_t key1,const uint32_t key2)
{	hash4str_elem** t= &h->elem[ key1 & h->mask ];
	
/*go down list*/
	while( *t!=NULL )
	{	if( (*t)->key1 == key1 && (*t)->key2 == key2 )
		{if( 0==strcmp((*t)->str,str) )
		{	return t; /*they are equal*/}}
	/*inequal, check next*/
		t= &(*t)->next;
	}
/*Here, we did not find it.*/
	return t;
}
/* Find element, if not found, returns a pointer to something NULL. (strings must terminate with '\0'!) */
hash4str_elem** hash_get_elem( hash4str* h, 
                                      const unsigned char len, const char* str)
{	uint32_t key1,key2;
	hashlittle2( str,len, &key1,&key2);
	return hash_get_elem_raw(h, len,str, key1,key2);
}


/* Same as get, but if not find it, make it. */
hash4str_elem** hash_obtain_elem_raw(hash4str* h, const unsigned char len, const char* str,
	const uint32_t key1,const uint32_t key2)
{	
	hash4str_elem** t= hash_get_elem_raw(h, len,str, key1,key2);
	if( *t==NULL ) /*case of not finding it*/
	{	h->cnt++;
		*t=hash_make_elem(len,str, key1,key2, h->extra_alloc);
	}
	return t;
}
hash4str_elem** hash_obtain_elem(hash4str* h, const unsigned char len, const char* str)
{	uint32_t key1,key2;
	hashlittle2( str,len, &key1,&key2);
	return hash_obtain_elem_raw(h, len,str, key1,key2);
}

void hash_ptr_remove_elem(hash4str_elem** r)
{/* Take element out list */
	hash4str_elem* del=*r; 
	*r = (*r)->next;
/* Delete */
	free( del->str );
	free( del );
}

/* Removes one element */
int hash_remove_elem_raw(hash4str* h, const unsigned char len, const char* str,
		const uint32_t key1,const uint32_t key2)
{	
	hash4str_elem** t= hash_get_elem_raw(h, len,str,  key1,key2);
	if( *t==NULL ){ return 0; }
	hash_ptr_remove_elem(t);
	return 1;
}
int hash_remove_elem(hash4str* h, const unsigned char len, const char* str)
{	uint32_t key1,key2;
	hashlittle2( str,len, &key1,&key2);
	return hash_remove_elem_raw(h, len,str, key1,key2);
}


/* Checks hash, moves elements where needed. */
int hash_check(hash4str* h)
{	int c=0,i=0;
	while( i< h->size )
	{	hash4str_elem** e=&h->elem[i];
		while( *e!=NULL )
		{if( ((*e)->key1& h->mask) !=i ) /*Compare index with where it should be*/
		{	c++;
		/* Should be elsewhere, take it out. */
	/*TODO this was wrong before, make list-macros, manual is error-prone*/
			hash4str_elem* t=*e;
			*e=(*e)->next; t->next=NULL;
			
		/* Put back elsewhere. */
			hash4str_elem** put_back=hash_get_elem_raw(h, t->len, t->str, t->key1,t->key2);
			if( *put_back!=NULL )
			{	printf("ERROR in hash_check\n"); }
			*put_back=t;
		}else{ e=&(*e)->next; }
		}
		i++;
	}
	return c;
}

/* Must be power of two!! */
void hash_init(hash4str* h, const uint32_t size, const int extra_alloc)
{
	h->extra_alloc=extra_alloc;
	h->elem= (hash4str_elem**)malloc(size*sizeof(hash4str_elem*) );
	h->cnt=0; h->size=size;h->mask=size-1;
	int i=0;
	while( i < 	size ){ h->elem[i]=NULL;i++; }
}

/* Increase size when it gets a lot of collisions. MUST BE POWER OF TWO!! */
void hash_increase_size(hash4str* h, const uint32_t size)
{	h->mask=size-1;
	h->elem=(hash4str_elem**)realloc((void*)h->elem, size*sizeof(hash4str_elem*));
	
	int i=h->size;
	while( i<size ){ h->elem[i]=NULL;i++; }
	h->size=size;
/* Will now be incorrect as hell. */
	hash_check(h);
}

/* Frees the hash*/
void hash_destr( hash4str* h )
{	int i=0;
	while( i<h->size )
	{	while( h->elem[i]!=NULL ) 
		{	hash4str_elem* del=h->elem[i];
			h->elem[i]=h->elem[i]->next;
			free( del->str );free( del );
		}
		i++;
	}
	free( (void*)h->elem );
}
