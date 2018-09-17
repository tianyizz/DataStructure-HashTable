/*
 *  Tianyi Zhang
 * 	ECE 2230
 * 	Dec 5th 2017
 *
 * 	This file includes all the function needed to
 * 	implement hash table functionalities. The function
 * 	in this file have direct access to the structure created in the
 * 	main program which contains the word and its count.
 */

#include<stdlib.h>
#include <stdio.h>
#include <string.h>
#include"list.h"
#define WSZ 128 //max word size
#define TABLE_SIZE 128 //max table size
#define MAX_STRING_SIZE 128 //max word size

struct hash_table
{
	list *table; /* an array of linked lists - dynamically allocated */
	int size;                   /* number of elements in the array */
	int curPos;							/* whatever else you need */

};

struct book_word
{
	char word[MAX_STRING_SIZE];   /* null terminated ascii key */
	int count[2];     /* 0 for the first book, 1 for the second book */
					  /* whatever else you need */
};

//initialize table
struct hash_table* hash_init();

//set pointer to first in the hash table
void* iterate_head(struct hash_table*h);

//set pointer to next item in the hash table
void* iterate_next(struct hash_table*h);

//insert item
int hash_insert(struct hash_table *h, char *key, void *data);

//search item
void *hash_search(struct hash_table *h, char *key);

//fill the table
int fillTable(FILE* ptr, struct hash_table* newTable,int bookNum, int* totalWord);

//get the hash number of a string
int hash(char *p, int m);

//destructor free memory
void hash_finalize(struct hash_table*);

//helper function
void eraseStr(char* words);

//helper function
size_t strnlen(const char *str, size_t maxlen);
