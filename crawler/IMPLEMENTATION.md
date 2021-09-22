# Will McCall
July 29, 2021

# Crawler implementation

## data structures and dependencies
Crawler is implemented according to the notes outlined in the DESIGN and REQUIREMENTS spec files and was designed and tested on plank.thayer.dartmouth.edu servers.  It uses the specific data structures `bag`, `hashtable`, and `webpage`, and `pagedir` which are defined in ../libcs50.

crawler also uses ../common functions described in pagedir.h:

```c
//modular function for fetching page HTML from page reference (once constructed)
void pagefetcher(webpage_t *webpage);
//modular function for parsing HTML for links to travel to next
void pagescanner(webpage_t *webpage, hashtable_t *table, bag_t *bag);
//function to help properly save webpage in appropriate format
void pagesaver(webpage_t *page, char *fname);
//modular function to help keep track of what function is doing/trying to do by printing to standard out.
void recorder(int level, char *command, char *link);
```

In the `main` function we validate parameters and initialize hashtable (tracker), bag (queue) and seed webpage, to pass to the crawler, returning a number of non zero exit codes for specific failures, and returning 0 after a successful call to crawler has terminated. 
we define the crawler function as follows:
```c
//takes depth, URL and directoryname and writes HTML contents of all fetched webpages to a certain depth in a specified file output
void crawler(int max, char* URLp, char* directoryname);
```
the `crawler` function expects the above 3 parameters, and executes the following algorithm:

## the algorithm 
The execution of the crawler algorithm takes place in the following format:

1. execute from a command line as shown in the User Interface
2. parse the command line, validate parameters, initialize other modules
3. make a *webpage* for the `seedURL`, marked with depth=0
4. add that page to the *bag* of webpages to crawl
5. add that URL to the *hashtable* of URLs seen
3. while there are more webpages to crawl,
	5. extract a webpage (URL,depth) item from the *bag* of webpages to be crawled,
	4. pause for at least one second,
	6. use *pagefetcher* to retrieve the HTML for the page at that URL,
	5. use *pagesaver* to write the webpage to the `pageDirectory` with a unique document ID, as described in the Requirements.
	6. if the webpage depth is < `maxDepth`, explore the webpage to find links:
		7. use *pagescanner* to parse the webpage to extract all its embedded URLs;
		7. for each extracted URL,
			8. 'normalize' the URL (see below)
			9. if that URL is not 'internal' (see below), ignore it;
			9. try to insert that URL into the *hashtable* of URLs seen
				10. if it was already in the table, do nothing;
				11. if it was added to the table,
					12. make a new *webpage* for that URL, at depth+1
					13. add the new webpage to the *bag* of webpages to be crawled



