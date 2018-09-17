/*
 * mainFunction.h
 *
 *  Created on: Dec 8, 2017
 *      Author: tianyiz
 *
 * The file contains the helper functions for the main
 * these function is defined separately from hash table functions or
 * list functions because they have the access to the actual file or
 * is directly related to solving the words problems.
 */


#ifndef MAINFUNCTION_H_
#define MAINFUNCTION_H_

#include<stdio.h>
/*
 * To fill the table
 *
 * */
int fillTable(FILE* ptr, struct hash_table* newTable,int bookNum, int* totalWord);

/*
 * free the whole table when process ends
 *
 * */

void free_table(struct hash_table*h);
/*
 * Count lines read when parsing
 * */
void count_lines(char* firstTxt, char*seconTxt, int* totaLines);

/*
 * Count number of unique words
 * */
void count_unique(struct hash_table* f, int* totalSize);

/*
 * count number of unique words for both input files
 * */
int count_bothUnique(struct hash_table*f);

/*
 * Find the 5 largest words in the document, if not enough
 * words are presented, the function will post all the words it found
 *
 * In the function, bookNum refers to the first or second book (0 or 1)
 * and both book (-1)
 * */
void largest_words(struct hash_table* f, int bookNum);

/*
 * Same structure as finding the largest five words
 *
 * */
void frequent_words(struct hash_table* f, int bookNum);
//same structure as the last two functions to calculate the least frequent words
void least_words(struct hash_table* f, int bookNum);


#endif /* MAINFUNCTION_H_ */
