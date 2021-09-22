#!bin/bash
# 
# Will McCall, CS50, August 6, 2021
# testing module for indexer and indextest
#

# representative test cases for both executables:

###########
#indexer:

# no args
./indexer

# bad directory
./indexer invaliddirectory testout

# good params:
./indexer ../crawler/t4 testout


############
#indextest:

# testing writable file
./indextest testout copytestout
