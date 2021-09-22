/*
 * Will McCall CS50
 * Querier takes pagedirectory and indexfilename and returns result of various search queries.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../common/pagedir.h"
#include "../common/word.h"
#include "../common/index.h"
#include "../libcs50/file.h"
#include "../libcs50/webpage.h"
#include "../libcs50/counters.h"
#include "../libcs50/memory.h"
#include "../libcs50/hashtable.h"


//prelim approved
//intersection functions
void scoreIntersect(void *param, int id, int count);
void scoreUnion(void *param, int id, int count);
//helper functions
void docCountDeleteHelp(void *param, int size);
void counting(void *param, int id, int count);
void sorter(void *param, int size);
void populator(void *param, int id, int count);

//query processing helpers
counters_t* andProcess(counters_t *left, counters_t *right);
counters_t* orProcess(counters_t *left, counters_t *right);

//query logic
char** readIn(FILE *fpoint);
void queryHandler(FILE *fpoint, char** tokenized, char* dirname, idx_t *dex);
counters_t *queryReader(idx_t *dex, char** tokenized);


//booleans
bool isOr(char *wordcheck);
bool isAnd(char *wordcheck);
bool validateCommands(int argc, char *argv[]);
bool isAlphaNumeric(char *wordcheck);

typedef struct twocts 
{
	counters_t *leftcount;
	counters_t *rightcount;
}  	twocts_t;

typedef struct keycount
{
int docid;
int score;
} keycount_t;


// takes document scores and combines on basis of intersection, expects two counters as arg, to be used in cases of AND
void scoreIntersect(void *param, int id, int count)
{
	//create the two counters structure and initialize instance variables.  Then, implement intersect logic outlined in activity.
	twocts_t *opcount = param;
	counters_t *left = opcount->leftcount;
	counters_t *right = opcount->rightcount;
	if (counters_get(right, id) > 0)
	{
		if (counters_get(right, id) <= count){
			// if right counter is less than count, set left to that new count
			counters_set(left, id, counters_get(right, id));
			
		}
		else{
			//otherwise set left to params.
			counters_set(left, id, count);

		}

	}

}


//performs document scoring on the basis of the union function (to be used in the case of OR)
void scoreUnion(void *param, int id, int count)
{

	counters_t *tobeset = param;
	//look if tobeset already created
	if (counters_get(tobeset, id) <= 0){
		//instantiate
		counters_set(tobeset, id, count);
	}

	else{
		//add to existing basis
		int basis = counters_get(tobeset, id);
		counters_set(tobeset, id , basis + count);
	}

}


void counting(void *param, int id, int count)
{
	int *sum = param;
	// verify correct parameters, make sure we are not passing in NULL and that ID is in range
	if (id <= 0 || sum == NULL)
	{
		
	}
	else
	{
		(*sum)++;
	}
}


//helper function to iterate over counterset and create a list of keycount objects containing doc information
void populator(void *param, int id, int count){	

	keycount_t **docs = param;
	keycount_t *ids = malloc(sizeof(keycount_t*));
	ids->docid = id;
	ids->score = count;
	int idx = 0;


	//iterate until reaching a valid document, which will be satisfied once docs[i] is NULL. Input new doc there.
	while (docs[idx] != NULL)
	{
		idx++;
	}
	docs[idx] = ids;
}


//quadratic sorter for input, fixes output to be in descending order.
void sorter(void *arg, int size)
{
	keycount_t **oparray = arg;
	
	// naive O(N^2) algorithm finds max of remaining scores and swaps it to ith position
	int i = 0;

	while (i < size)
	{
		int j = 0;
		while (j < size)
		{
			if (oparray[j]->score >= oparray[i]->score)
 			{


			}
			else
			{
				//perform swap
				keycount_t *swap = oparray[i];
				oparray[i] = oparray[j];
				oparray[j] = swap;
				
			}

			j+=1;
		}
		i+=1;
	}

}



//takes reference do doc key/ doc count array and corresponding legnth.  free each element and reference to array as well.
void docCountDeleteHelp(void *param, int size)
{
	keycount_t **docidlist = param;
	int i = 0;
	while (i < size)
	{
	free(docidlist[i]);
	i +=1;

	}

	free(docidlist);

}


// uses score intersection function to  'and' process queries.
counters_t* andProcess(counters_t *left, counters_t *right)
{
	//create a new counter to contain result of intersection
	counters_t *validcounters = counters_new();
	//as suggested in 
	twocts_t both = {validcounters, right};
	//iterate over counterset scanning into left element of the twocounter object such that the final result contains the intersection value.
	counters_iterate(left, &both, scoreIntersect);
	return validcounters;

}

//uses score union function to process 'or' queries
counters_t *orProcess(counters_t *left, counters_t *right)
{
	counters_t *validcount = counters_new();
	counters_iterate(right, validcount, scoreUnion);
	counters_iterate(left, validcount, scoreUnion);
	
	return validcount;
}

//implements query and/or logic and creates a counter set based on it

counters_t *queryReader(idx_t *dex, char** tokenized)
{
	counters_t *last;
	counters_t *current;
	

	//iterate through tokenized query input, word by word, perform intersect or union on relevant counterset depending on
	//parameters
	int i = 0;
	while (tokenized[i] != NULL)
	{

		// if we land on or...
		if (isOr(tokenized[i]))
		{
			//increment to next word
			i+= 1;
			//if the word exists 
			if ((tokenized[i]) != NULL)
			{
				//find the word in the index hashtable to process
				current = hashtable_find(getIdx(dex), tokenized[i]);
				last = orProcess(last, current);

			}

			else
			{
				//otherwise, instantiate new counterset and unionize.
				current = counters_new();
			}
		}

		else
		{
	
			if(isAnd(tokenized[i]))
			{
				//increment to next word
				i+=1;
			}

			//if word exists find index value of word, process against last
			if (tokenized[i] != NULL)
			{
				
				current = hashtable_find(getIdx(dex), tokenized[i]);
				//handle special case
				if (i == 0)
				{
					last = current;
				}
				last = andProcess(last, current);

			}

			else

			{
				current = counters_new();
				//handle special case
				if (i == 0)
				{
					last = current;
				}
			}

		}

		i+=1;
	}
	

	return last; 
}

//query subcommand for checking if a word parameter is the word "or"
bool isOr(char *wordcheck)
{
	if (strcmp("or",wordcheck) != 0)
	{
		return false;
	}
	else
	{
		return true;

	}

}

//modular command for checking if a query word is "and"
bool isAnd(char *wordcheck)
{

	if (strcmp("and", wordcheck) != 0)
	{
		return false;
		
	}
	else
	{
		return true;
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
			//now lets check if our file argument has writing permission by instantiating a filepointer and attempting to open argv[2] for writing
			FILE *fp1;
			if ((fp1 = fopen(argv[2], "r")) != NULL)
			{
			
				fclose(fp1);
			}	

			else{

				fprintf(stderr, " argument %s is not writeable, check usage ./querier directory indexfilename\n", argv[2]);
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


//desinged to take a filepointer as input and tokenize 
char** readIn(FILE *fpoint)
{
	
	
	printf("your query: ");
	printf("\n");
	char *input = freadlinep(fpoint);
	char **tokenized = calloc(1000, sizeof(char));

	//if any of these cases are true, we can simply return NULL.
	if ((input == NULL || strcmp(input,"") == 0) || !isAlphaNumeric(input))
	{
		free(input);
		return NULL;

	}
	else 
	{
		//otherwise we are alphanumeric, so we can normalize and tokenize our input before returning it.
		//use common word library to normalize input word
		input = wordNormalizer(input);

		
			// give plenty of room for tokenized words
		
			char *current= strtok(input, " ");
			tokenized[0] = current;
			int i  = 1;
			//continue tokenizing from same initial input string until all words have been tokenized
			while ((current = strtok(NULL," ")))
			{
				tokenized[i] = current;
				i++;
			}


			return tokenized;

	}

	
}


// takes tokenized query input, prints documents in ranked order by score
void queryHandler(FILE *fpoint, char** tokenized, char* dirname, idx_t *dex)
{
	//read file line by line where tokenized represents most recent tockenized line read.
	while (tokenized[0] != NULL && tokenized != NULL)
	{
		
		//check if first word is and or or.  This cannot happen, so if it does, exit nonzero
		if (isAnd(tokenized[0]) || isOr(tokenized[0]))
		{
			printf("error! first query word can't be 'and' or 'or'");
			exit(-1);
		}

		int i = 1;

		while (tokenized[i] != NULL)
		{

			//starts from index 1, goes until null word, checks if successive words are query words and exits nonzero if they are
			if ((isAnd(tokenized[i]) || isOr(tokenized[i])) && (isAnd(tokenized[i-1]) || isOr(tokenized[i-1])))
			{
				printf( "error! can't have successive occurences of query words 'and' or 'or'");
				exit(-2);
			}

			i+=1;
		}
		i = i-1;

		//now we have progressed to the final non null query entry, this also cannot be and or or, so if it is, exit nonzero.
		if (isAnd(tokenized[i]) || isOr(tokenized[i]))
		{
			printf("error! first query word can't be 'and' or 'or'");
			exit(-3);

		}

		printf("query read as - ");

		// print out tockenized query input 
		int j = 0;
		while (tokenized[j] != NULL)
		{
			printf("%s, ", tokenized[j]);
			j +=1;
		}

		printf("\n");

		//create the appropriate counterset based on index and tokenized input.
		counters_t *qcts = queryReader(dex, tokenized);

		int size = 0;
		//iterate over counterset to determine number of matches
		counters_iterate(qcts,&size,counting);
		printf("found %d files matching search parameters in directory! \n", size);
		
		//now create an array of documents with scores which can store up to 'size' documents
		keycount_t **docarray = calloc(size,  sizeof(keycount_t));

		counters_iterate(qcts,docarray,populator);
		//run algo to sort docarray in descending.
		sorter(docarray, size);

		//loop to print out document info, we have ID and score, we need URLS
		int k = 0;
		while (k < size)
		{
			//create directory path with extra room, copy over directory name, append /docid
			char *directoryPath = calloc((strlen(dirname) + 10), sizeof(char));
			strcpy(directoryPath, dirname);
			strcat(directoryPath, "/");

			char docIdBuff[1000];
			sprintf(docIdBuff, "%d", docarray[k]->docid);
			strcat(directoryPath, docIdBuff);

			//to retrieve URL, we open this file for reading, read the first line into variable retrieved
			FILE *URLretriever = fopen(directoryPath, "r");
			char* retrieved = freadlinep(URLretriever);
			fclose(URLretriever);

			fprintf(stdout, "final score: %d ID: %d URL: %s \n", docarray[k]->score, docarray[k]->docid, retrieved);
			free(directoryPath);
			free(retrieved);

			k+=1;
		}
		


		printf("\n");
		printf("\n");
		docCountDeleteHelp(docarray, size);
		counters_delete(qcts);
		tokenized = readIn(fpoint);
	}

}


//used to validate that all tokens are alphanumeric.
bool isAlphaNumeric(char *wordcheck)
{
	char *check = wordcheck;
	
	while (*check != '\0')
	{
		if (isalpha(*check) || isspace(*check))
		{

		}
		else
		{
			fprintf(stderr, "invalid query input character! ");
			return false;

		}

		check++;
	}
	return true;
}


int main(int argc, char *argv[])
{
	if (!validateCommands(argc, argv))
	{
		exit(1);
	
	}

	else
	{
		//create references to filepointer and directory/indexfile to be used in queries
		FILE *fp;
		char *dexfile = argv[2];
		char *directory = argv[1];
		char **wordarray;
		idx_t *dex = newIdx(lines_in_file(fp = fopen(dexfile, "r")));
		fclose(fp);
		loadIdx(dex, dexfile);
		//process queries
		wordarray = readIn(fp = stdin);
		queryHandler(fp, wordarray, directory, dex);
		//memory cleanup
		delIdx(dex);
		free(wordarray);
		fclose(fp);
		exit(0);
		
	}
}