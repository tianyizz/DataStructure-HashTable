/*
 *  Tianyi Zhang
 * 	ECE 2230
 * 	Dec 5th 2017
 */
#include"hash.h"
#include"mainFunction.h"
#include<unistd.h>
#include<getopt.h>
#include<string.h>

//main function
int main(int argc, char **argv){
	int a;
	//assume largest file name is 100 characters;
	char* infileF=(char*)calloc(100,sizeof(char));
	if(infileF==NULL){
		printf("Program unable to allocate memory for file name\n");
		return 0;
	}
	char* infileS=(char*)calloc(100,sizeof(char));
	if(infileS==NULL){
		free(infileF);
		printf("Program unable to allocate memory for file name\n");
		return 0;
	}

	//parsing flags
	while((a = getopt (argc, argv, "f:s:")) != -1){
		switch(a){
			case 'f':
				strcpy(infileF, optarg);
				break;

			case 's':
				strcpy(infileS, optarg);
				break;

			case '?':
				if(optopt == 'f'||optopt == 's')
					printf("Options -f and -s require an argument.\n");
				else{
					printf("Unknown characters.  Usage is ./a.out -f FIRSTFILE -s SECONDFILE\n");
				}
				if(infileF!=NULL)free(infileF);
				if(infileS!=NULL)free(infileS);
				return 0;
		}
	}

	//open file pointers
	FILE *ptrF=fopen(infileF,"r");
	if(ptrF==NULL){
		printf("Program unable to open the first file \n");
		free(infileF);
		free(infileS);
		return 0;
	}

	FILE *ptrS=fopen(infileS,"r");
	if(ptrS==NULL){
		printf("Program unable to open the second file\n");
		fclose(ptrF);
		free(infileF);
		free(infileS);
		return 0;
	}


	//initializing variables
	struct hash_table* newTable=hash_init();
	int totalWord[2]={0};
	int totalLine[2]={0};
	int totalUnique[2]={0};


	if(newTable==NULL){
		printf("Program unable to allocate memory for new table\n");
		return 0;
	}

	//parsing first book
	if(fillTable(ptrF,newTable,0,totalWord)==0){
		printf("Unable to fill the first table. Program terminating.\n");
		fclose(ptrF);
		fclose(ptrS);
		free(infileF);
		free(infileS);
		free_table(newTable);
		return 0;
	}

	//parsing second book
	if(fillTable(ptrS,newTable,1,totalWord)==0){
		printf("Unable to fill the second table. Program terminating.\n");
		fclose(ptrF);
		fclose(ptrS);
		free(infileF);
		free(infileS);
		free_table(newTable);
		return 0;
	}

	//counting lines and unique words
	count_lines(infileF, infileS, totalLine);
	count_unique(newTable, totalUnique);

	//print results for the first book
	printf("%s\n\n",infileF);
	printf("Number of lines processed \t\t%d\n",totalLine[0]);
	printf("Number of words processed \t\t%d\n",totalWord[0]);
	printf("Number of Unique words \t\t\t%d\n\n",totalUnique[0]);
	largest_words(newTable,0);
	frequent_words(newTable,0);
	least_words(newTable,0);

	//print results for the second book
	printf("%s\n\n",infileS);
	printf("Number of lines processed \t\t%d\n",totalLine[1]);
	printf("Number of words processed \t\t%d\n",totalWord[1]);
	printf("Number of Unique words \t\t\t%d\n\n",totalUnique[1]);
	largest_words(newTable,1);
	frequent_words(newTable,1);
	least_words(newTable,1);

	printf("Common Vocabulary (appears in both)\n\n");
	printf("Number of Unique words \t\t\t%d\n\n",count_bothUnique(newTable));
	largest_words(newTable,-1);
	frequent_words(newTable,-1);
	least_words(newTable,-1);

	//cleanup
	fclose(ptrF);
	fclose(ptrS);
	free(infileF);
	free(infileS);
	free_table(newTable);
}
