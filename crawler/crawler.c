/*
* Will McCall
* July 27, 2021
* crawler.c implements functionality outlined in DESIGN.md by constructing a modular set of functions to handle input checks and crawl webpages, while fetching, scanning, or saving them.
*
*
*/

//relevant headerfiles for crawler to sufficiently execute
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../common/pagedir.h"
#include "../libcs50/webpage.h"
#include "../libcs50/hashtable.h"
#include "../libcs50/memory.h"
#include "../libcs50/bag.h"
#include <math.h>

//note: please consult pagedir.h for further information on helper functions implemented throughout this file.  To see the code, consult pagedir.c


// implements 13 step crawler search logic defined in design/implementation spec. see main() below.
void crawler(int max, char* URLp, char* dirname){
	
	/* we're going to need to instantiate hashtable and bag to serve as a reference for what has been seen, and what comes next in queue.
	* but first we need to create our seed webpage
	* seed page has depth 0, no HTML, and seed URL by default.
	*/
	webpage_t *seed = webpage_new(URLp, 0, NULL);


	/*hypothesis: for strongly connected graphs the function of n reachable nodes is n ~ x^d depth, so to keep hashtable sparsely populated we need slots to increase exponentially too.
	*since 2^10 = 1024 (not unreasonable), we will cap ht size at 2^10 (from main() function limits) and to ensure optimal performance we will take the min of 64 and 2^n for low d.\
	*/

	//instantiate a hashtable where the number of slots is exponentially proportional to the search depth
	hashtable_t *catcher;
	
	//determine hashtable as hybrid max of exponential function and 64 slots
	int min = 64;
	int cmp = pow(2,max);

	if (min < cmp)
	{
		min = cmp;
	}

	catcher = hashtable_new(min);

	//when inserting into the hashtable we don't care about the value, so we put an empty string.
	hashtable_insert(catcher, webpage_getURL(seed), "");


	bag_t *processor;
	processor = bag_new();
	bag_insert(processor, seed);
	
	webpage_t *incrementpage;

	// every page is going to need its own page ID
	int pageid = 1;
	
	while ((incrementpage = bag_extract(processor)) != NULL){

		//create buffer, read page ID into buffer, append to form final address in a minute	
		char c[5];
		sprintf(c, "%d", pageid);
		// construct file path starting with dirname/pageid
		char *fpointer = (char*)calloc((strlen(dirname) + 4), sizeof(char*));	
		strcpy(fpointer, dirname);
		strcat(fpointer, "/");
		strcat(fpointer, c);



		printf("creating file %s...\n",fpointer);
		pagefetcher(incrementpage);

		if ((webpage_getHTML(incrementpage)) == NULL){
			
			// if we cant find the HTML for this page, we just ignore it and keep moving to next iteration of loop.
			recorder(webpage_getDepth(incrementpage), "non existant file...", webpage_getURL(incrementpage));
			webpage_delete(incrementpage);
			//free(fpointer);
			continue;
		}
		//save 
		pagesaver(incrementpage, fpointer);
		pageid++;

		//check if pagedepth is valid for scanning, if so, run scan on it, 
		if (webpage_getDepth(incrementpage) < max){
			recorder(webpage_getDepth(incrementpage),"scanning...", webpage_getURL(incrementpage));

			pagescanner(incrementpage, catcher, processor);
		}
		webpage_delete(incrementpage);
	}
	//clean up helper data structures
	bag_delete(processor, webpage_delete);
	hashtable_delete(catcher, NULL);
}


/*does basic argument verification, checking if input in valid range and correct number of arguments. If so, prepares starting variables and runs crawler algorithm
*  0 if success
* -1 if URL not internal
* -2 if input directory non existant
* -3 if invalid arg number
* -4 if invalid depth range [0,10]
* -5 unknown error (should never happen)
*/ 
int main(const int argc, char *argv[]){

	
	if (argc == 4 && atoi(argv[3]) >= 0 && atoi(argv[3]) <= 10){

	//create pointers for URL and directoryname and prepare max parameter to be passed by value to crawler function.
	//be sure to 
	char *directoryname = (char*)count_malloc( (strlen(argv[2]) + 9 + 1));
	char *URLp = (char*)count_malloc(( strlen(argv[1])) + 1);
	int max = atoi(argv[3]);

	//copy directory name
	strcpy(directoryname, argv[2]);
	//with extra space we're going to create path for a file called .crawler into our directory
	strcat(directoryname, "/.crawler");
	//we're also going to copy over the input URL into a local variable for ease of operations.
	strcpy(URLp, argv[1]);


	//lets also make sure our input directory name exists and is writable (we do this by attempting to open for writing, which will only be allowed if we have directory permission)
	//to do this we put an empty file .crawler which will also be useful in future labs

	FILE *file_pointer;
	if((file_pointer = fopen(directoryname, "w")) != NULL)
	{
		//no further use for .crawler
		fclose(file_pointer);
		printf("directory exists and is readable!\n");
		//if we pass this check, we still have to make sure that our URL is internal
		if (IsInternalURL(URLp) == true)
		{
			//success! we have a valid directory and an internal URL.
			printf("success! URL is internal!\n");
		}
		else

			{
			//oops, URL not internal
			fprintf(stderr, "Careful! URL is not internal, for the sake of respect your url's must start with 'http://cs50tse.cs.dartmouth.edu' \n");	
		
			// make valgrind happy
			count_free(URLp);
			count_free(directoryname);
			exit(-1);

		}
	}
	else
	{
		
		fprintf(stderr, "Error checking your input directory...  Are you sure it exists and is writeable? \n");

		// make sure not to get any unecessary memory errors
		count_free(URLp);
		count_free(directoryname);
		exit(-2);

	}
	
	//now that we have validated input arguments its time to actually run the crawler algorithm
	printf("crawling URL %s to depth %d and storing results in %s\n", URLp, max, argv[2]);

	crawler(max, URLp, argv[2]);
	//keep valgrind happy
	count_free(directoryname);
	
	//code 0 means successful run
	exit(0);

	}

	else
	{
		//improper argument formatting or out of range.
		fprintf(stderr, "invalid args.  Either incorrect order (./crawler URL directory depth), wrong number of args, or depth out of range [0,10]\n");
		
		if(argc != 4)
		{
			exit(-3);
		}
		if(atoi(argv[3]) < 0 || atoi(argv[3]) > 10)
		{
			exit(-4);
		}
		exit(-5);
	}
}
