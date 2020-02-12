CC = g++
ARGS = -Wall -pthread -o
BINARIES= input

all: ${BINARIES}

input: input.cpp
	${CC} ${ARGS} input input.cpp

clean:
	rm -f ${BINARIES}