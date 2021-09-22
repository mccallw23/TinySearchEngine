## Will McCall
August 17, 2021

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


# Assumptions

The querier assumes that the input directory and files follow the designated formats:

* `pageDirectory` has files named 1, 2, 3, ..., without gaps.
* The content of files in `pageDirectory` follow the format as defined in the specs
* The content of the file named by `indexFilename` follows the [Index file format]({{site.labs}}/lab5/REQUIREMENTS.html#index-file-format)
