#Will McCall
#CS50, July 28, 2021
#makefile for crawler module


CFLAGS= -Wall -pedantic -std=c11 -ggdb -I../libcs50 -I../common
CC = gcc

PROG = crawler
OBJS = crawler.o 
LIBS = ../libcs50/libcs50-given.a ../common/common.a

.PHONY: all test clean

#fixed to link with math library

$(PROG): $(OBJS) $(LIBS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o $(PROG) -lm

crawler.o: ../libcs50/bag.h ../libcs50/hashtable.h ../libcs50/webpage.h ../common/pagedir.h

# testing 
test:
	bash -x ./testing.sh

# cleaning up
clean:
	rm -rf *.dYSM
	rm -rf *~ *.o
	rm -rf $(PROG)