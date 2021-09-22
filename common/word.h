/*
 *
 * Will McCall CS50
 * word.h contains header definitions for word.c, which serves the purpose of normalizing words to lowercase
 */


#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../libcs50/memory.h"



//takes an input word and returns it's lowercase analogue
char* wordNormalizer(char *wordop);