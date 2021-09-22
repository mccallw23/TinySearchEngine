# Will McCall 
August 5, 2021

## Design

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

