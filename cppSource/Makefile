all: main.o variSwap.o
	icpc -o variSwap main.o variSwap.o
main.o:
	icpc -g -O3 -Wall -c main.cpp
variSwap.o:
	icpc -g -O3 -Wall -c variSwap.cpp

cleanup:
	rm *.o
	rm variSwap
