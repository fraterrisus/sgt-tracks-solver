CC=g++
CXXFLAGS=-std=c++11 -g

TARGETS=solver.o board.o square.o

default: solver

board.o: square.h

solver.o: board.h

solver: $(TARGETS)

clean:
	rm -f $(TARGETS)
