# Tiny Search Engine
## Will McCall
July 23, 2021

GitHub username: mccallw23

To build, run `make`.

To test functionality, run `make test`

To clean up, run `make clean`.


The Tiny Search Engine Crawls and Indexes webpages, and handles user Queries for relevant information. It was designed and implemented using a modular approach.  Run the commands above in order to test and run the whole program, check subdirectories for directions on modular use.


# TSE Crawler Design Spec

Recall the [Crawler Requirements Spec](REQUIREMENTS.md); the **crawler** crawls a website and retrieves webpages starting with a specified URL.
It parses the initial webpage, extracts any embedded URLs and retrieves those pages, and crawls the pages found at those URLs, but limits itself to `maxDepth` hops from the seed URL and to URLs that are 'internal' to the designated CS50 server.
When the crawler process is complete, the indexing of the collected documents can begin.

A Design Spec should contain several sections:

* User interface
* Inputs and Outputs
* Functional decomposition into modules
* Pseudo code for logic/algorithmic flow
* Dataflow through modules
* Major data structures
* Testing plan

Let's look through each.

### User interface

The crawler's only interface with the user is on the command-line; it must always have three arguments.

```
crawler seedURL pageDirectory maxDepth
```

For example:

``` bash
$ ./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html data 2
```

### Inputs and outputs

Input: the only inputs are command-line parameters; see the User Interface above.

Output: We save each explored webpage to a file, one file per page.
We use a unique document ID as the file name, for document IDs 1, 2, 3, 4, and so forth.
Within a file, we write

 * the page URL on the first line,
 * the depth of the page (where the seed is depth 0) on the second line,
 * the page contents, beginning on the third line.

### Functional decomposition into modules

We anticipate the following modules or functions:

 1. *main*, which parses arguments and initializes other modules
 2. *crawler*, which loops over pages to explore, until the list is exhausted
 3. *pagefetcher*, which fetches a page from a URL
 4. *pagescanner*, which extracts URLs from a page and processes each one
 4. *pagesaver*, which outputs a page to the the appropriate file

And some helper modules that provide data structures:

 1. *bag* of pages we have yet to explore
 4. *hashtable* of URLs we've seen so far

### Pseudo code for logic/algorithmic flow

The crawler will run as follows:

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

**normalize the URL** means to convert it into a clean, canonical form.
As one simple example, 
`Http://CS50TSE.cs.dartmouth.edu/tse/letters/index.html`
becomes
`http://cs50tse.cs.dartmouth.edu/tse/letters/index.html`.

**internal** means the URL stays within the CS50 playground `http://cs50tse.cs.dartmouth.edu/`.

A good implementation will not necessarily encode all the above code in a single, deeply-nested function; part of the Implementation Spec is to break the pseudocode down into a cleanly arranged set of functions.

Notice that our pseudocode says nothing about the order in which it crawls webpages; since it presumably pulls them out of a *bag*, and a *bag* abstract data structure explicitly denies any promise about the order of items removed from a bag, we can't expect any particular crawl order.
That's ok.
The result may or may not be a Breadth-First Search, but for the crawler we don't care about the order as long as we explore everything within the `maxDepth` neighborhood.

The crawler completes and exits when it has nothing left in its bag - no more pages to be crawled.
The maxDepth parameter indirectly determines the number of pages that the crawler will retrieve.


### Dataflow through modules

 1. *main* parses parameters and passes them to the crawler.
 2. *crawler* uses a bag to track pages to explore, and hashtable to track pages seen; when it explores a page it gives the page URL to the pagefetcher, then the result to pagesaver, then to the pagescanner.
 3. *pagefetcher* fetches the contents (HTML) for a page from a URL and returns.
 4. *pagesaver* outputs a page to the appropriate file.
 4. *pagescanner* extracts URLs from a page and returns one at a time.

### Major data structures

Three helper modules provide data structures:

 1. *bag* of page (URL, depth) structures
 2. *set* of URLs (indirectly used by hashtable)
 4. *hashtable* of URLs

### Testing plan

*Unit testing*.  A small test program to test each module to make sure it does what it's supposed to do.

*Integration testing*.  Assemble the crawler and test it as a whole.
In each case, examine the output files carefully to be sure they have the contents of the correct page, with the correct URL, and the correct depth.
Ensure that no pages are missing or duplicated.
Print "progress" indicators from the crawler as it proceeds (e.g., print each URL explored, and each URL found in the pages it explores) so you can watch its progress as it runs.

0. Test the program with various forms of incorrect command-line arguments to ensure that its command-line parsing, and validation of those parameters, works correctly.

0. Test the crawler with a `seedURL` that points to a non-existent server.

0. Test the crawler with a `seedURL` that points to a non-internal server.

0. Test the crawler with a `seedURL` that points to a valid server but non-existent page.



# Indexer Design

The index module is designed to take as input a page directory and index file name in the format `./index pagedirectory indexfilename` This module then outputs the data to indexfilename where each line of the file contains data about word frequency in the form `word docID count [docID count]...`

## Functionality
This module includes functionality for:
1. instantiating an index
2. building an index from a crawler directory
3. saving an index
4. deleting an index
5. inserting into an index
6. loading an index from an index file


## Implementation

The module was designed with ease of debugging in mind with a strong emphasis on modularity.  For this reason, the module's main function is a composition of a few other functions each with a sub goal required to successfully write the file output. The module first validates the command line arguments, and if successful, iterates through each file, finding valid words (above length 3), indexing words and thier corresponding counts.  It then saves the contents of this index into a file, and finally deletes the index, returning exit code 0 if successful.


## data structures
The module uses data structures: `counters` and `hashtable`


## Querier Design

querier.c is activated by the command ./querier directory indexfile after it is made using `make`.

It is designed to execute from a command line with usage syntax `./querier pageDirectory indexFilename`
    where `pageDirectory` is the pathname of a directory produced by the Crawler, and
   *where `indexFilename` is the pathname of a file produced by the Indexer.

   To do this, querier

1. validates its command-line arguments:
	* `pageDirectory` is the pathname for an existing directory; if it cannot be verified to be a directory produced by the Crawler, the indexer shall print an error message and exit non-zero. 
	* `indexFilename` is the pathname of a readable file; if it is not possible to open it for reading, the querier should print an error message and exit non-zero.

2. loads the index from `indexFilename` into an internal data structure.

3. reads search queries from stdin, one per line, until EOF.
	4. clean and parse each query according to the *syntax* described below.
	5. if the query syntax is somehow invalid, print an error message, do not perform the query, and prompt for the next query.
	5. print the 'clean' query for user to see.
	5. use the index to identify the set of documents that *satisfy* the query, as described below.
	6. if the query is empty (no words), prints nothing.
	6. if no documents satisfy the query, prints `No documents match.`
	7. otherwise, ranks the resulting set of documents according to its *score*, as described below, and print the set of documents in decreasing rank order; for each, list the score, document ID and URL.
(Obtain the URL by reading the first line of the relevant document file from the `pageDirectory`.)
	8. Exits with zero status when EOF is reached on stdin.


## Assumptions

The querier assumes that the input directory and files follow the designated formats:



