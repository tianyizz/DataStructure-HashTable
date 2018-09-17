/*
 * mainFunction.c
 *
 *  Created on: Dec 8, 2017
 *      Author: tianyiz
 */
#define _GNU_SOURCE
#include<string.h>
#include"hash.h"
#include"mainFunction.h"

/*
 * To fill the table
 *
 * */
int fillTable(FILE* ptr, struct hash_table* newTable,int bookNum, int* totalWord){
	char tempStr[MAX_STRING_SIZE];
	struct book_word* wordNode;
	int result;

	//keep scanning words from the file
	while(fscanf(ptr," %s",tempStr)==1){
		//recording total word number in a book
		totalWord[bookNum]++;

		//search if the word exists
		wordNode=hash_search(newTable,tempStr);

		//if didn't find any existing word, create a new one
		if(wordNode==NULL){
			wordNode=(struct book_word*)calloc(1,sizeof(*wordNode));

			if(wordNode==NULL){
				printf("Unable to allocate memory for the new node\n");
				return 0;
			}

			strcpy(wordNode->word,tempStr);
			wordNode->count[bookNum]=1;

			result=hash_insert(newTable,tempStr,wordNode);

			if(result==0){
				printf("Program failed to insert node\n");
				return 0;
			}

		}
		//increment the original count
		else{
			(wordNode->count[bookNum])++;
		}

		eraseStr(tempStr);
	}

	return 1;
}

/*
 * free the whole table when process ends
 *
 * */

void free_table(struct hash_table*h){
	if(h==NULL)return;


	struct book_word* temp=iterate_head(h);

	for(;temp!=NULL;temp=iterate_next(h)){
		//printf("%s\n",temp->word);
		free(temp);
	}

	hash_finalize(h);
}

/*
 * Count lines read when parsing
 * */
void count_lines(char* firstTxt, char*seconTxt, int* totaLines){
	FILE *ptrF=fopen(firstTxt,"r");
	FILE *ptrS=fopen(seconTxt,"r");
	char* line=NULL;
	size_t len=0;

	while(getline(&line,&len,ptrF)!=-1)
		totaLines[0]++;

	while(getline(&line,&len,ptrS)!=-1)
		totaLines[1]++;

	if(line!=NULL)free(line);

	fclose(ptrF);
	fclose(ptrS);
}

/*
 * Count number of unique words
 * */
void count_unique(struct hash_table* f, int* totalSize){
	if(f==NULL)return;


	struct book_word* temp=iterate_head(f);

	for(;temp!=NULL;temp=iterate_next(f)){
		if(temp->count[0]>0)totalSize[0]++;
		if(temp->count[1]>0)totalSize[1]++;
	}

}

/*
 * count number of unique words for both input files
 * */
int count_bothUnique(struct hash_table*f){
	if(f==NULL)return 0;
	int i=0;

	struct book_word* temp=iterate_head(f);

	for(;temp!=NULL;temp=iterate_next(f)){
		if(temp->count[0]>0&&temp->count[1]>0)
		i++;
	}

	return i;
}

/*
 * Find the 5 largest words in the document, if not enough
 * words are presented, the function will post all the words it found
 *
 * In the function, bookNum refers to the first or second book (0 or 1)
 * and both book (-1)
 * */
void largest_words(struct hash_table* f, int bookNum){

	printf("5 largest words\n");

	struct book_word* i[5]={NULL};
	int a=0;

	struct book_word* temp=iterate_head(f);

	for(;temp!=NULL;temp=iterate_next(f)){

		//filling pointer array when it is not full yet (with 1 book)
		if(a<5&&(i[a]==NULL)&&bookNum!=-1&&(temp->count[bookNum]>0))i[a++]=temp;

		//filling pointer array when it is not full yet (with both books)
		else if(a<5&&i[a]==NULL&&bookNum==-1&&(temp->count[0]>0)&&(temp->count[1]>0))i[a++]=temp;

		//array is full, filling requires compare
		else if(a>4){
			for(a=0;a<5;a++){
				//when there is one book
				if(bookNum!=-1&&strlen(temp->word)>strlen(i[a]->word)
						&&(temp->count[bookNum]>0)){
					int tempMin=0;
					int b;
					for(b=1;b<5;b++)
						if(strlen(i[b]->word)<strlen(i[tempMin]->word))
							tempMin=b;
					i[tempMin]=temp;
					a=5;
					break;
				}
				//when compare both book
				if(bookNum==-1&&strlen(temp->word)>strlen(i[a]->word)
						&&(temp->count[0]>0)&&(temp->count[1]>0)){
					int tempMin=0;
					int b;
					for(b=1;b<5;b++)
						if(strlen(i[b]->word)<strlen(i[tempMin]->word))
							tempMin=b;
					i[tempMin]=temp;
					a=5;
					break;
				}
			}
		}
	}

	/*
	 * printing out results
	 * */
	for(a=0;a<5;a++){
		if(i[a]!=NULL)
		printf("\t%s\t\t\t%d\n",i[a]->word,(bookNum==-1?i[a]->count[0]+i[a]->count[1]:i[a]->count[bookNum]));
	}

	//if nothing was in the text
	if(i[0]==NULL)printf("Nothing has been entered.\n");

	printf("\n");
}

/*
 * Same structure as finding the largest five words
 *
 * */
void frequent_words(struct hash_table* f, int bookNum){
	printf("15 most frequently used words\n");

	struct book_word* i[15]={NULL};
	int a=0;

	struct book_word* temp=iterate_head(f);

	for(;temp!=NULL;temp=iterate_next(f)){

		//filling empty array
		if(a<15&&i[a]==NULL&&bookNum!=-1&&(temp->count[bookNum]>0))i[a++]=temp;
		else if(a<15&&i[a]==NULL&&bookNum==-1&&(temp->count[0]>0)&&(temp->count[1]>0))i[a++]=temp;

		//if array is filled and needs compare
		else if(a>14){
			for(a=0;a<15;a++){

				//one book
			 if(bookNum!=-1&&(temp->count[bookNum])>(i[a]->count[bookNum])){
				 int tempMin=0;
				 int b;
				 for(b=1;b<15;b++)
				 if(i[b]->count[bookNum]<i[tempMin]->count[bookNum])
					tempMin=b;
				i[tempMin]=temp;
				a=15;
				break;
			 }

			 //both book
			 if(bookNum==-1&&((temp->count[0]+temp->count[1])>(i[a]->count[0]+i[a]->count[1]))){
				 int tempMin=0;
				 int b;
				 for(b=1;b<15;b++)
				 if((i[b]->count[0]+i[b]->count[1])<(i[tempMin]->count[0]+i[tempMin]->count[1]))
					tempMin=b;
				i[tempMin]=temp;
				a=15;
				break;
			 }
			}
		}
	}

	//print function
	for(a=0;a<15;a++){
		if(i[a]!=NULL)
		printf("\t%s\t\t\t%d\n",i[a]->word,(bookNum==-1?i[a]->count[0]+i[a]->count[1]:i[a]->count[bookNum]));
	}

	if(i[0]==NULL)printf("Nothing has been entered.\n");

	printf("\n");
}

//same structure as the last two functions to calculate the least frequent words
void least_words(struct hash_table* f, int bookNum){
	printf("5 least frequently used words\n");

	struct book_word* i[5]={NULL};
	int a=0;

	struct book_word* temp=iterate_head(f);

	for(;temp!=NULL;temp=iterate_next(f)){

		if(a<5&&i[a]==NULL&&bookNum!=-1&&bookNum!=-1&&(temp->count[bookNum]>0))i[a++]=temp;
		else if(a<5&&i[a]==NULL&&bookNum==-1&&(temp->count[0]>0)&&(temp->count[1]>0))i[a++]=temp;
		else if(a>4){
			for(a=0;a<5;a++){
			 if(bookNum!=-1&&(temp->count[bookNum])<(i[a]->count[bookNum])
				&&(temp->count[bookNum]>0)){
				 int tempMin=0;
				 int b;
				 for(b=1;b<5;b++)
				 if(i[b]->count[bookNum]>i[tempMin]->count[bookNum])
					tempMin=b;
				 i[tempMin]=temp;
				 a=5;
				break;
			 }

			 if(bookNum==-1&&
			   ((temp->count[0]+temp->count[1])<(i[a]->count[0]+i[a]->count[1]))&&
			   temp->count[0]>0&&temp->count[1]>0){
				 int tempMin=0;
				 int b;
				 for(b=1;b<5;b++)
				 if((i[b]->count[0]+i[b]->count[1])>(i[tempMin]->count[0]+i[tempMin]->count[1]))
				 	tempMin=b;
				 i[tempMin]=temp;
				 a=5;
				 break;
			}
			}
		}
	}

	for(a=0;a<5;a++){
		if(i[a]!=NULL)
		printf("\t%s\t\t\t%d\n",i[a]->word,(bookNum==-1?i[a]->count[0]+i[a]->count[1]:i[a]->count[bookNum]));
	}

	if(i[0]==NULL)printf("Nothing has been entered.\n");

	printf("\n");
}
