# Will McCall
July 29, 2021

## Information

the common library is required to build and use the crawler and indexer modulse, and will eventually be required to use querier as well.  

`pagedir.c` holds helper functions for the crawler module in fetching, scanning, and saving pages.
`word.c` helps normalize words to lowercase for indexer
`index.c` defines the index datatype and operations that can be made on it.

in order to prevent any problems with other modules, make sure that you run `make` before attempting to use the TSE.