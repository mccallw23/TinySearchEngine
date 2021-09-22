# Will McCall
# July 29, 2021

OBJS = pagedir.o 

LIB = common.a

CFLAGS = -Wall -pedantic -std=c11 -ggdb -I../libcs50
CC = gcc

# Building lib
$(LIB): $(OBJS)
	ar cr $(LIB) $(OBJS)

# object files depend on the header files
pagedir.o: ../libcs50/webpage.h pagedir.h 

index.o: index.h pagedir.h ../libcs50/webpage.h ../libcs50/hashtable.h ../libcs50/file.h ../libcs50/counters.h

word.o: word.h

.PHONY: clean  

# clean up
clean:
	rm -f *~ *.o
	rm -rf *.dSYM
