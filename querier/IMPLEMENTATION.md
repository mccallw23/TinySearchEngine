# Will McCall
August 17, 2021

The Implementation of querier made heavy use of the principal of modularity. The `main` function in querier.c first validates command line input before loading in the passed index, and then processing the query based on the passed index and directory.  For more details, check the DESIGN.md file.

