# Will McCall
# Makefile for querier


CC = gcc
CFLAGS = -Wall -pedantic -std=gnu11 -ggdb -I ../libcs50 -I ../common 
LLIBS = ../libcs50/libcs50-given.a ../common/common.a


querier: querier.o
		$(CC) $(CFLAGS) querier.o $(LLIBS) ../common/word.c ../common/index.c -o querier

fuzzquery: fuzzquery.o
		$(CC) $(CFLAGS) fuzzquery.o $(LLIBS) -o fuzzquery


.PHONY: clean test 

clean:
	rm -rf querier
	rm -rf fuzzquery 
	rm -rf *.o


test: 
	bash -x testing.sh
