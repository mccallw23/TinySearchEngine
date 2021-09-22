# Will McCall 
# August 2020
# Makefile builds CS50 Tiny search engine indexer.

CFLAGS= -Wall -pedantic -std=c11 -ggdb -I../common -I../libcs50
CC=gcc
LLIBS = ../libcs50/libcs50-given.a ../common/common.a


.PHONY: all clean test

all: indexer indextest

indexer: indexer.o  ../common/pagedir.o ../common/index.o ../common/word.o $(LLIBS)
	$(CC) $(CFLAGS) indexer.o  ../common/pagedir.o ../common/index.o ../common/word.o  $(LLIBS) -o indexer

indextest: indextest.o ../common/index.o $(LLIBS)
	$(CC) $(CFLAGS) indextest.o ../common/index.o $(LLIBS) -o indextest

test:
	bash -x testing.sh

clean:
	rm -rf *.dSYM
	rm -rf *~ *.o
	rm -rf indexer indextest