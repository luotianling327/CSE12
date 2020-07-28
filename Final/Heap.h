//==========================================================================
// cs12wi20cu                         Homework 10              Tianling Luo
//--------------------------------------------------------------------------
// File: Heap.h
//
// Description: In this assignment, we will implement a heap data
// structure to use the "template" feature of C++. 
//==========================================================================

#ifndef HEAP_H
#define HEAP_H

#include <iostream>
#include "Item.h"
using namespace std;

//=========================================================================
// class Tree
//
// Description: The Tree class is defined and it contains the member
// functions that will be called by the Tree.c code.
//
// Data Fields:
//     size (int) - size of the array
//     arr (Item**) - array that contains heap items
//     occupancy (long)  - the occupancy in the tree
//     debug (static int) - option to set debug message
//
// Functions:
//     Heap - constructor
//     ~Heap - destructor
//     static void Set_Debug_On - set debug on
//     static void Set_Debug_Off - set debug off
//     swapItem - swap two items in heap
//     unsigned long Insert - Insert nodes into the tree
//     unsigned long Remove - Remove nodes from the tree
//     Write - Write in in-order traversal
//     reheapUp - go up the tree
//     reheapDown - go down the tree
//==========================================================================

class Heap: public Item{
	private:
		int size;//size of the array
		Item ** arr;// array for heap
		long occupancy;//occupancy
		static int debug;//for debug option

		void swapItem(int, int);//swap two items
		void reheapUp(int);//go up the tree
		void reheapDown(int);//go down the tree

	public:
		static void Set_Debug_On(void);//debug on
		static void Set_Debug_Off(void);//debug off
		Heap(int);//constructor
		~Heap(void);//destructor
		unsigned long Insert(Item *);//insert function
		Item* Remove(void);//remove function
		ostream & Write(ostream &);//write function
};

#endif
