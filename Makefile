## definitions
CC = g++
CCFLAGS = -Wall
OBJS1 = scanner.o token.o main.o symboltable.o admin.o parser.o first.o set.o blocktable.o assembler.o interp.o

## targets and prerequisites
.PHONY : all
all : main

main: $(OBJS1)

prog1 : $(OBJS1)
	$(CC) $^ -o $@

# default rule for compiling .cc to .o
%.o: %.cpp                               ## next line must begin with a TAB
	$(CC) -c $(CCFLAGS) $<
 

## generate the prerequistes and append to the desired file
.prereq : $(OBJS1:.o=.cpp) $(wildcard *.h) Makefile
	rm -f .prereq
	$(CC) $(CCFLAGS) -MM $(OBJS1:.o=.cpp) >> ./.prereq 

## include the generated prerequisite file
include .prereq

.PHONY : clean
clean :                 ## next lines must begin with a TAB
	rm -f *.o
	rm -f *~ *# .#*

.PHONY : clean-all
	clean-all : clean      ## next line must begin with a TAB
	rm -f main