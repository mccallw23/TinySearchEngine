/*
 *
 * Will McCall CS50
 *
 * word.c has helper function wordNormalizer which takes as input any string and outputs it as it's lower case analogue
 */

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../libcs50/memory.h"


char* wordNormalizer(char *wordop){

	char *retword = malloc(strlen(wordop) + 1);
	strcpy(retword, wordop);

	int i = 0;
	while (i < strlen(wordop))
	{

		retword[i] = tolower(wordop[i]);

		i++;
	}

	free(wordop);
	return retword;

}

