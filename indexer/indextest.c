
/*
 * Will McCall, CS50, 
 * 
 * indextest.c tests index.c by building a new index from the output file and then writing that index into another output file.  Successful if file contents are identical
 * 
 */

//standard inclusions
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

//additional library inclusions
#include "../common/index.h"
#include "../libcs50/memory.h"
#include "../libcs50/file.h"



//indextest 
bool validateCommands(int argc, char *argv[])
{

	//validate arguments are correct
	if (argc == 3)
	{

		//if so, move to attempting to openning the file passed in by the first argument
		FILE *fp;
		//check readability of file.
		if ((fp = fopen( argv[1] , "r") ) != NULL)
		{
			fclose(fp);

			// if check passes, check writability of 2nd arg
			if ((fp = fopen( argv[2] , "w")) != NULL)
			{
				fclose(fp);
			}	

			else
			{
				fprintf(stderr, "second argument is unwriteable!");
				return false;
			}
		}

		else
		{

			fprintf(stderr, "error! file %s is unreadable", argv[1]);
			return false;
		}
	}
	else
	{
		fprintf(stderr, "error! invalid argument count! \n ");
		return false;

	}

	//if we make it past these 3 validation checks our arguments pass!
	return true;

}

int main(int argc, char *argv[])
{
	//make sure arguments 
	if (!validateCommands(argc, argv))
	{
		exit(-1);
	}
	else
	{
		//create a new index, load it from argument 1, load it into argument 2, delete it, and exit successfully! 
		idx_t *idx = newIdx(200);
		loadIdx(idx, argv[1]);
		saveIdx(idx, argv[2]);
		delIdx(idx);
		exit(0);


	}
}