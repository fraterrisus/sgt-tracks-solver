CC=g++
CXX=g++
CXXFLAGS=-std=c++11 -g
GENERATE=../puzzles/tracks --generate 1 10x10

TARGETS=solver.o board.o square.o

default: solver

board.o: square.h

solver.o: board.h

solver: $(TARGETS)

clean:
	rm -f $(TARGETS)

runtest: solver
	$(GENERATE) > test
	./$< < test > log
	cat log
