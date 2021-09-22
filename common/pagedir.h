/*
 * Will McCall
 * July 28, 2021
 * pagedir.h immplements all function definitions for pagedir.c which handles webpage operations
 * for the tiny search engine.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../libcs50/webpage.h"
#include "../libcs50/memory.h"
#include "../libcs50/hashtable.h"
#include "../libcs50/bag.h"
/********** pagesaver **********/


//will be used to execute crawler algorithm in design spec
void crawler(int max, char* URLp, char* directoryname);
//modular function for fetching page HTML from page reference (once constructed)
void pagefetcher(webpage_t *webpage);
//modular function for parsing HTML for links to travel to next
void pagescanner(webpage_t *webpage, hashtable_t *table, bag_t *bag);
//function to help properly save webpage in appropriate format
void pagesaver(webpage_t *page, char *fname);
//modular function to help keep track of what function is doing/trying to do by printing to standard out.
void recorder(int level, char *command, char *link);