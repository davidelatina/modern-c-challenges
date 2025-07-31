CFLAGS = -std=c11 -ggdb3 -Wall -Wpedantic -Og
OPT_MACROS = -DDEBUG_MSG_ON
CC=gcc
LIBS = -lm

all: \
	1-SequentialSortingAlgorithms \
	2-NumericalDerivatives \
	3-PiDigits \
	4-Union-Find \
	5-ComplexDerivatives \
	7-UndirectedGraph

1-SequentialSortingAlgorithms:
	gcc $(CFLAGS) -o $@ 1-SequentialSortingAlgorithms.c

2-NumericalDerivatives:
	gcc $(CFLAGS) -o $@ 2-NumericalDerivatives.c $(LIBS)

3-PiDigits:
	gcc $(CFLAGS) -o $@ 3-PiDigits.c

4-Union-Find:
	gcc $(CFLAGS) -o $@ 4-Union-Find.c

5-ComplexDerivatives:
	gcc $(CFLAGS) -o $@ 5-ComplexDerivatives.c $(LIBS)

7-UndirectedGraph:
	gcc $(CFLAGS) -o $@ 7-UndirectedGraph.c


