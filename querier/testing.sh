# Will McCall, 
# testing shell script for querier.c uses file testwords to run a few representative test cases on querier.


## incorrect number of arguments

./querier /thayerfs/courses/21summer/cosc050/cs50tse.old/tse-output/toscrape-depth-2 

## invalid directory

./querier /thayerfs/courses/21summer/cosc050/cs50tse.old/tse-output/toscrape-depth /thayerfs/courses/21summer/cosc050/cs50tse.old/tse-output/toscrape-index-2 

## invalid index name

./querier /thayerfs/courses/21summer/cosc050/cs50tse.old/tse-output/toscrape-depth-2 /thayerfs/courses/21summer/cosc050/cs50tse.old/tse-output/toscrape-index

## representative test cases in testwords.c

./querier /thayerfs/courses/21summer/cosc050/cs50tse.old/tse-output/toscrape-depth-2 /thayerfs/courses/21summer/cosc050/cs50tse.old/tse-output/toscrape-index-2 < testwords