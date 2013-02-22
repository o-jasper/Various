
#include "../hash4str.c"
#include <stdio.h>

void hash_tokens(struct hash4str* h, FILE* f,
		const char* delimiters)
{	
	char cur[256];int i=0, cnt=0;
		
	while( 1 )
	{	int j=-1;
		while( j<0 )
		{	fread(cur+0, 1,1, f); if( feof(f) || ferror(f) ){ return; }
			j=0;
			while( delimiters[j]!='\0' && j>=0 )
			{	if( cur[0]==delimiters[j] ){ j=-2;break; } j++; }
		}
		i=1;
		while( i<256 && !feof(f) && ! ferror(f) )
		{	fread(cur+i, 1,1, f); if( feof(f) || ferror(f) ){ return; }
			j=0;
			while( delimiters[j]!='\0' && j>=0 )
			{	if( cur[i]==delimiters[j] ){ j=-2;cur[i]='\0';break; } j++; }
			
			if( j<0 )
			{	
			/* Add in hash.(if not there yet) */
				(*(int*)hash_elem_item(*hash_obtain_elem(h, i, cur)))++;
				break;
			}
			else{ i++; }
		}	
	}
}

void print_hash( struct hash4str* h )
{	int i=0;
	while( i < h->size )
	{	struct hash4str_elem* iter=h->elem[i];
		while( iter!=NULL )
		{	printf("%s : %d\n",iter->str, *(int*)hash_elem_item(iter));
			
			iter=iter->next;
		}
		i++;
	}
}

int main( int argc, char* argv[] )
{	
	struct hash4str h;
	hash_init(&h, 2048, 4);
	
	printf("Get hashing.\n");
        if( argc!=2 )
        {   printf("Incorrect number of args exitting\n"); return -1; }

	FILE* f=fopen( argv[1], "r");
        if( f==NULL || ferror(f) )
        { printf("File didnt open or something.\n"); return -1; }
           
	hash_tokens(&h,f, " \t\n',.:;()[]*+-\`~!\"\"/?");
	if( h.cnt>h.size )
	{	printf("Need resize from %d", h.size);
		int newsize=h.size;
		while( newsize<=h.cnt )
		{	newsize*=2; }
		printf(" to %d.\n", newsize);
		hash_increase_size(&h, newsize);
	}
	
	fclose(f);
	printf("Done hashing, print.\n");
	print_hash(&h);
	printf("Deleting hash, hash-size now %d, count %d.\n", h.size,h.cnt);
	
	hash_destr(&h);
	return 0;
}
