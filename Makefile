CC=g++
CXX=g++
CXXFLAGS=-std=c++11 -g
GENERATE=../puzzles/tracks --generate 1 10x10

TARGETS=solver.o board.o board_solvers.o square.o path.o

default: solver

prof: CXXFLAGS+= -pg
prof: LDFLAGS+= -pg
prof: solver

debug: CXXFLAGS+= -DDEBUG
debug: solver

vdebug: CXXFLAGS+= -DVERBOSE
vdebug: debug

board.o: square.h

solver.o: board.h

solver: $(TARGETS)

clean:
	rm -f $(TARGETS)

runtest: solver
	$(GENERATE) > test
	./$< < test > log
	cat log
