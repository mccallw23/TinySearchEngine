/*
 *
 * Will McCall CS 50
 *
 * index.c implements indexer functionality outlined in spec and further described in headerfile index.h
 */


//standard inclusions
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
//common
#include "../common/pagedir.h"
#include "../common/word.h"
#include "../common/index.h"
//libcs50
#include "../libcs50/webpage.h"
#include "../libcs50/memory.h"
#include "../libcs50/jhash.h"
#include "../libcs50/file.h"
//data structures
#include "../libcs50/hashtable.h"
#include "../libcs50/counters.h"

//headers for internal helper functions...

void saveIdxFunc(void *arg, const char *key, void *value);
void delIdxFunc(void *todel);
void saveCounters(void *arg, int key, int num);


typedef struct idx
{
	//an index is going to consist of a hashtable 
	hashtable_t *table;

}idx_t;


//instanntiates new hashtable in index of size tableSize
idx_t *newIdx(int tableSize)
{
	//make a new index by instantiating index hashtable to be of size tablesize.
	idx_t *idx = count_malloc(sizeof(idx_t));
	idx->table = hashtable_new(tableSize);
	return idx;

}


//finds value associated with key in index
void *findIdx(idx_t *idx, char *key)
{
	//find index table and run hashtable find on it with the corresponding key 
	hashtable_t *idxtable = idx->table;
    return hashtable_find(idxtable, key);
	
}


bool insertIdx(idx_t *idx, char *key, void *value)
{
	//we simply want to return the success result of insertion of the key value pair into the index table
	hashtable_t *idxtable = idx->table;
	return hashtable_insert(idxtable, key, value);
}



void saveIdx(idx_t *idx, char *name)
{
	//printf("here");
	//open file by filename for writing
	FILE *fp;
	fp = fopen(name, "w");

	//iterate over index table and use helper function to save appropriately
	hashtable_t *idxtable = idx->table;
	hashtable_iterate(idxtable, fp, saveIdxFunc);

	fclose(fp);
}


void loadIdx(idx_t *idx, char *name)
{
	//we need to read in every word.  One assumption made is that no word will be longer than 200.
	char word[200];

	//once we read integers from the file, we won't be sure if they are an ID or count at first, so we simply store in variable file Int
	int fileint;
	//atId will determine if we are at an ID, since file format is listed in such a way that it always alternates, we can properly read into ID or counter variable depending on boolean.
	bool atId = true;

	// integers to read fileint into if we are either at a count or an ID...
	int fileid;
	int wordcount;

	// every time we read in an ID/Count pair we need to initialize a counter which keeps track of 
	// id/count pairs for a given word. to do this, we will also 	
	counters_t *ctrop;
	int idcountpair = 1;
	

	//we need to open the file name that was passed in for reading
	FILE *fp;
	fp = fopen(name, "r");

	//iterate over every individual file word
	//read that word into word
	while(fscanf(fp, "%s", &word[0]) == 1)
	{
		
		//make new counter for this word
		ctrop = counters_new();
		
		// while we continue to see numbers, read them into file int variable.
		while (fscanf(fp, "%d", &fileint) == 1)
		{
			// we need to determine if we're at an ID or a count, which we can do with the boolean below.
			if (!atId)
			{
				// then we must be at a count, make word count equal to the integer we read in.
				idcountpair = idcountpair + 1;
				wordcount = fileint;
				
			}
			else
			{
				//next time we will be at a count
				idcountpair = idcountpair + 1;
				fileid = fileint;

			
			}


			// if above 2, we have read both a fileid and count and must now update our data structure
			if (idcountpair > 2)
				{
					//once this variable exceeds 2 it means we have processed and read a file ID and its corresponding count, so we must reset it for next loop
					//All that's left to do is set the counter accordingly and insert it into the hashtable with a key corresponding to the word buff that was read in...
					idcountpair = 1;
					counters_set(ctrop, fileid, wordcount);
						
				}
			//reverse boolean to reflect transition between ID and count.
			atId = !atId;
		}
		
		hashtable_t *tabledex = idx->table;
		hashtable_insert(tabledex, word, ctrop);


	}
	
	fclose(fp);

}


void delIdx(idx_t *idx)
{
	hashtable_t *idxtable = idx->table;
	hashtable_delete(idxtable, delIdxFunc);
	free(idx);
}


//helper functions for deleting and saving...
//helper for delIdx, takes 
void delIdxFunc(void *todel)
{
	//we will only ever be passing counters to this function though it must be declared void...
	counters_t* delete = todel;
	counters_delete(delete);
}

//helper function to saveIdx which takes output file, key/value, and saves values of wordcounter with corresponding key to output file in specified format
void saveIdxFunc(void *arg, const char *key, void *value)
{
	FILE *fpoint = arg;
	counters_t *ctrop = value;
	fprintf(fpoint, "%s ", key);

	counters_iterate(ctrop, fpoint, saveCounters);
	//as we save we want one word per line, so once done with iteration put a newline
	fprintf(fpoint, "\n");
}

//passed into savIdxFunc as helper to print counter contents to file
void saveCounters(void *arg, int key, int num)
{
	//take file pointer as arg and print output in specified format for design spec.
	FILE *fpoint = arg;
	fprintf(fpoint, "%d %d ", key,  num);

}

