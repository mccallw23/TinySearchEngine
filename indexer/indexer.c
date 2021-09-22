/*
 * Will McCall, CS 50, August 4
 * indexer.c implements logic from spec file, validating arguments before building, saving, and deleting an index data structure.
 *
 */

//standard inclusions
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
// common library
#include "../common/pagedir.h"
#include "../common/index.h"
#include "../common/word.h"
// data structures
#include "../libcs50/counters.h"
#include "../libcs50/hashtable.h"
// additional library files of use
#include "../libcs50/memory.h"
#include "../libcs50/file.h"
#include "../libcs50/webpage.h"


//modular function to validate inputs to main function
bool validateCommands(int argc, char *argv[]);
//modular function to build index in main function
void idxBuilder(idx_t *idx, char *dir);

/*
* main function 
* 1) validates commands
* 2) builds an index from crawler directory
* 3) saves index to output file argv[2]
* 4) deletes index
* 5) exits 0 for success, nonzero error.
*/ 
int main(int argc, char *argv[])
{

	if (!validateCommands(argc, argv))
	{
		exit(-1);
	}
	
	else{
	
		// we want to keep this index sparsely populated to ensure optimal lambda value in hashtable
		idx_t *idx = newIdx(100);
		

		// we want to build the index, save its content to an output file that we can read later, and then delete the index, exiting with code 0 for 
		// success if all operations succeed.
		idxBuilder(idx, argv[1]);
		saveIdx(idx, argv[2]);

		delIdx(idx);
		exit(0);

	}

}

bool validateCommands(int argc, char *argv[])
{
	//first validate arg count
	if (argc == 3)
	{
		//proceed to checking if we are in a crawler directory...
		//create path to crawler directory using a character buffer, check for presence of .crawler file, by attempting to open for reading, if successful proceed.
		char path[150];
		strcpy(path,argv[1]);
		strcat(path , "/");
		strcat(path , ".crawler");
	
		//Try to open .crawler for reading
		FILE *fp;

		if((fp = fopen(path , "r")))
		{
			fclose(fp);
			//now lets check if our file argument has writing permission by instantiating a filepointer and attempting to open argv[2]
			FILE *fp1;
			if ((fp1 = fopen(argv[2], "w")) != NULL)
			{
				fclose(fp1);
			}	

			else{

				fprintf(stderr, " argument %s is not writeable, check usage ./indexer directory filename\n", argv[2]);
				return false;

				}
		
		}
		else
		{
			fprintf(stderr, " %s invalid crawler directory! are you sure you want to be looking here? \n", argv[1]);
			return false;
		}
		
	
	}
	else
	{
		fprintf(stderr, " invalid number of arguments! \n");

		return false;
	}



	return true;

}

void idxBuilder(idx_t *idx, char *dir)
{
	//build path to particular folder by reading file ID into a string buffer and attatching it to the directory path
	char buff[200];
	//store id as integer, starting at 1, crawler assumes no missing file numbers in crawler directory
	int fileid = 1;
	
	//read in file ID onto the buffer with plans to add it to end of file path
	sprintf(buff, "%d", fileid);

	//were going to now active build a string path which we will allow to have up to 200 characters more than the directory (in keeping with buffer maximum)
	char* path= calloc(sizeof(char), strlen(dir) + 200);

	sprintf(path, "%s/%d", dir, fileid);

	// initialize new file pointer for opening whatever path leads to
	FILE *fp3;

	// we're going to need to operate on the webpage object for the purposes of leveraging getnextword.
	webpage_t *page;  

	//while we continue opening non null files
	while ((fp3 = fopen(path, "r")) != NULL)
	{
		//to check whats happening in terminal print path.
		printf("%s\n", path);
      
		//we're now going to begin parsing file information

		int currlocation = 0;

		char *wordop;
		char *url;
		url = freadlinep(fp3);

		//the 2nd line is depth info, which we don't need...
		//freeing prevents compile time warning of unused variable :P 
		char *depth = freadlinep(fp3);
		free(depth);
		
		//instantiate page from URL, begin reading from current location
		page = webpage_new(url, currlocation, freadfilep(fp3));

        //printf("looping start build\n");
		while (( wordop = webpage_getNextWord(page, &currlocation)) != NULL)
		{

           // printf("looping as expected in build\n");
		   // we need to account for the fact that a 4 character string is ok due to invisible null character
			if (strlen(wordop) >= 4)
			{
				counters_t *ctr;
				wordop = wordNormalizer(wordop);
			
				if (findIdx(idx, wordop) != NULL)
				{
					//printf("index found");
					ctr = findIdx(idx, wordop);
					
				}
				else
				{
					ctr = counters_new();
					
				}
				//build counter and insert it into the index.
				counters_add(ctr, fileid);
				insertIdx(idx, wordop, ctr);
			}

			free(wordop);

		}

		//increment ID to make next path
		fileid +=1;

		//delete and free
		webpage_delete(page);
		fclose(fp3);
		//update path to next ID.
		sprintf(path, "%s/%d", dir, fileid);
		
	}
	free(path);

}	





