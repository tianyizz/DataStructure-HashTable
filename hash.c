/*
 *  Tianyi Zhang
 * 	ECE 2230
 * 	Dec 5th 2017
 */

#include <stdbool.h>
#include <fcntl.h>
#include"hash.h"

/*
 * A help function as strnlen
 *
 * */
size_t strnlen(const char *str, size_t maxlen)
{
     size_t i;
     for(i = 0; i < maxlen && str[i]; i++);
     return i;
}

/*
 * Hash function provided by Dr.ligon
 * */
int hash(char *p, int m){
	int flip = 0;
	int len = 0;
	int s = 0;
	int shortcnt = 0;
	unsigned short *sp = NULL;
	len = strnlen(p, WSZ);
	shortcnt = len / 2;
	sp = (unsigned short *)p;
	unsigned short hval = 0x5ab6;
	/* folding the string by twos */
	for (s = 0; s < shortcnt; s++){
		unsigned short newv;
		newv = *sp;
		if (flip)
		{
			newv <<= 1;
		}
		flip = !(flip);
		hval ^= newv;
		sp++;
	}
	/* if odd size string double up last char */
	if (len & 0x0001)
	{
		hval ^= ((((short)(p[len-1])) << 8) | (short)(p[len-1]));
	}
	return (int)hval % m;
}

void eraseStr(char* words){
	while(*(words))
		*words++='\0';
}

struct hash_table* hash_init(){
	struct hash_table* newTable=(struct hash_table*)calloc(1,sizeof(struct hash_table));
	if(newTable==NULL)return newTable;

	newTable->size=TABLE_SIZE;
	newTable->curPos=0;
	newTable->table=(list*)calloc(TABLE_SIZE,sizeof(list));
	if(newTable->table==NULL)return NULL;

	int i=0;

	for(;i<TABLE_SIZE;i++){
		newTable->table[i]=list_init();
		if((newTable->table[i])==NULL) return NULL;
	}

	return newTable;
}

void* iterate_head(struct hash_table*h){
	int i=0;
	struct book_work* cur;

	//keep searching for the first word in the table
	for(;i<h->size;i++){
		cur=list_first(h->table[i]);
		if(cur!=NULL)break;
	}

	if(cur!=NULL)h->curPos=i;
	return cur;
}

void* iterate_next(struct hash_table*h){

		if(h->curPos>=TABLE_SIZE)return NULL;

		struct book_word* temp=list_next(h->table[h->curPos]);

		if(temp==NULL){

			for(++h->curPos;h->curPos<h->size;h->curPos++){
				temp=list_first(h->table[h->curPos]);
				if(temp!=NULL)break;
			}
		}
		return temp;
}



int hash_insert(struct hash_table *h, char *key, void *data){

	int keyHash=hash(key,h->size);
	struct book_word* result=list_append(h->table[keyHash],data);
	if(result==NULL) return 0;
	return 1;
}

void *hash_search(struct hash_table *h, char *key){
	if((h->size)<=0)h->size=2;
	int temp=hash(key,h->size);
	struct book_word* i=list_first(h->table[temp]);

	for(;i!=NULL;i=list_next((h->table[temp])))
		if(strcmp(i->word,key)==0) return i;

	return NULL;
}

void hash_finalize(struct hash_table*h){
	int i=0;

	if(h==NULL)return;
	for(;i<TABLE_SIZE;i++){
		list_finalize(h->table[i]);
	}

	free(h->table);
	free(h);
}

