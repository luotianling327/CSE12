#==========================================================================
# cs12wi20cu                         Homework 10              Tianling Luo
#--------------------------------------------------------------------------
# File: Makefile
#
# Description: In this assignment, we will contain Makefile commands 
# function: make new, make Driver, make valgrind
#==========================================================================

Driver: Driver.o Heap.o
	g++ -o Driver -g Driver.o Heap.o

Driver.o: Item.h Driver.c Heap.h SymTab.h
	g++ -g -c Driver.c

Heap.o: Item.h Heap.c Heap.h SymTab.h
	g++ -g -c Heap.c

clean:
	rm -f *.o Driver

new:
	make clean
	make

valgrind: Driver
	valgrind --leak-check=full --read-var-info=yes \
		--show-reachable=yes --track-origins=yes ./Driver $(DEBUG)





