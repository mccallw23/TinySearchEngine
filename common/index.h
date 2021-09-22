/*
 * 
 *
 * Will McCall CS 50
 *
 * index.h serves as the headerfile for index.c
 */

//standard libraries
#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <stdbool.h>

//additional libraries
#include "../libcs50/memory.h"
#include "../libcs50/webpage.h"
#include "../libcs50/hashtable.h"
#include "../libcs50/counters.h"
#include "../common/word.h"
#include "pagedir.h"



// structure definition for index datatype 
typedef struct idx idx_t;

//newIdx takes as input a table size and instantiated instance variable hashtable to contain that many slots.
idx_t* newIdx(int tableSize);

 //returns the item in the index according to the key, returns pointer if key is found, NULL otherwise
void *findIdx(idx_t *idx, char *key);

// returns result of insertion of key value pair into the index.
bool insertIdx(idx_t *idx, char *key, void *value);

//save index to output file
void saveIdx(idx_t *idx, char *name);

//load index from input file

void loadIdx(idx_t *idx, char *name);

//delete index based on index reference
void delIdx(idx_t *idx);



