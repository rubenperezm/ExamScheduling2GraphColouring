CXX = g++
CXXFLAGS = -std=c++14 -Wall -O3

EXES = solver
OBJS = ${EXES:=.o} solver.o

%: %.o
	${CXX} ${LDFLAGS} -o $@ $^

all: ${EXES}

run:
	./solver

${OBJS}: generator.hpp graph.hpp translator.hpp

clean:
	${RM} ${EXES} ${OBJS} core *~
