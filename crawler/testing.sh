
# Will McCall
# July 28, 2021
# testing.sh tests crawler module using a number of representative test cases


#################
#make directories
rm -rf t1 t2 t3 t4 t5 t6
mkdir t1 t2 t3 t4 t5 t6

##################
# bad parameters

# no args
./crawler

# one arg
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/

# two args 
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/ t1

# three args, illigitimate depth
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/ t1 -10

###############

#depth 0
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/ t1 0

#depth 1
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/ t2 1

#depth 2
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/ t3 2

#depth 6
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/ t4 6

######
# test different URL:

#depth 1
./crawler http://cs50tse.cs.dartmouth.edu/tse/toscrape/ t5 1


