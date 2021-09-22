
/* Will McCall
* July 29, 2021
* pagedir.c helps supply functionality to TSE, and is currently set up to help the crawler module with page operations 
* and console logging for easier debugging.
* 
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../libcs50/webpage.h"
#include "../libcs50/memory.h"
#include "../libcs50/hashtable.h"
#include "../libcs50/bag.h"


// a modular function to help keep track of what is happening internally by logging when a particular action is performed by the program
void recorder(int level, char *command, char *link){

	printf("Depth: %d: %s @URL %s \n", level, command, link);
}


// modular function for webpage to store HTML in page based on already existing URL. 
// see ../libcs50/webpage.h for function details.
void pagefetcher(webpage_t *webpage){

	webpage_fetch(webpage);
	recorder(webpage_getDepth(webpage), "Webpage Fetched", webpage_getURL(webpage));
	
}


// passes in reference to queue and seen table
void pagescanner(webpage_t *webpage, hashtable_t *table, bag_t *bag){
	
	
	char *URLref;
	int pos= 0;
	while ((URLref = webpage_getNextURL(webpage, &pos)) != NULL){
		if (IsInternalURL(URLref)){
			if (hashtable_insert(table, URLref, "")){

				//initialize and fetch a new webpage being processed
				webpage_t *processpage;
				//make depth one greater than reference page
				processpage  = webpage_new(URLref, webpage_getDepth(webpage)+1, NULL);
				bag_insert(bag, processpage);
				recorder(webpage_getDepth(webpage), "seeing/inserting to queue", webpage_getURL(processpage));
			}
			else{

				//couldn't insert, log duplicate and, free pointer, and iterate
				recorder(webpage_getDepth(webpage), "duplicate page", URLref);
				free(URLref);
			}
		}

		else{
			//non internal check and memory free
			recorder(webpage_getDepth(webpage), "not internal", URLref);  
			free(URLref);

		}

	
	}


}



//open whatever file was passed and in the format URL, DEPTH, HTML on separate lines (\n \n \n) write the file output
void pagesaver(webpage_t *webpage, char *fileref){
	
	//open file for writing based on file reference
	FILE *fpointer = fopen(fileref, "w");
	assertp(fpointer, "bad file \n");

	//write file based on pointer and formatting information
	fprintf(fpointer, "%s \n%d \n%s",webpage_getURL(webpage),webpage_getDepth(webpage),webpage_getHTML(webpage));

	//free pointers no longer needed/ in use at function termination..
	fclose(fpointer);
	free(fileref);

}           




