# Will McCall
August 6, 2021


# Overview
the indexer module is designed to read a preprocessed crawler directory and build an index containing information about the frequency of different words on different webpages throughout 
the crawler directory, which it outputs into a file using `saveIdx`.  It also has functionality to load an index from an index file, which will be useful when building the querier module.

# Usage

In order to use this module, you must ensure that this module and all of its dependencies have been compiled.  One way to do this would be to navigate to the parent directory and run `make`.  alternatively, make sure that the `common` directory has been compiled before running `make` in this directory.  To test the code, you can run `make test` and to input your own arguments you can run ./indexer pagedirectory outfile to test the indexing, and ./indextest existingindexfile newindexfile to load an index from an index file and write it to another file in order to make sure that loading works correctly.

# Assumptions

This module assumes that libcs50 and common modules have been compiled.  It also assumes that any crawler directory has file Id's starting at 1 and moving upwards with no gaps between numbers.