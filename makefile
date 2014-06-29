###########################################################
# File Name     : makefile
# Author        : Katie Brigham, Shawn Vega
# Class Accounts: masc,1420, masc1440
# Class         : CS 570, Summer 2014
# Assignment    : Assignment 3
# Description   : Programming Assignment 2 in C++
###########################################################
EXEC =a3
CC =g++
SOURCES=A3.cpp
FLAGS=-pipe

$(EXEC): 
	$(CC) -o $(EXEC) $(SOURCES) $(FLAGS)
	rm -f *.o

clean:
	rm -f *.o core a.out $(EXEC)

#######################[ EOF: Makefile ]###################