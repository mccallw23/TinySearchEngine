# Will McCall
August 6, 2021

## Overview:

The index module looks through a crawler made directory of webpage HTML content, finding all words above 3 characters in length, and cataloguing how many times each word appears on each file webpage. It is able to save this information to a file, and rebuild it into an index if needed.

## Mechanics

The indexer builds itself by looking through the crawler directory, reading each file by file ID, line by line, finishing only when there are no words left to process on the HTML page. The indexer saves information about the crawler directory word frequencies in an output file by iterating over every item in its internal hashtable to print out words and thier corresponding frequencies for every file ID in the crawler directory. the index module can also load information from an index file back into an index data structure using the index loading functionality.

## Data Types

In this module we implement index.c, which uses `hashtable` and `counters` and thus indirectly implements `set` through `hashtable`.

