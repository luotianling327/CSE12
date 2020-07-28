//==========================================================================
// cs12wi20cu                         Homework 10              Tianling Luo
//--------------------------------------------------------------------------
// File: SymTab.h
//
// Description: In this assignment, we will implement a heap data
// structure in C++.
//==========================================================================

#ifndef SYMTAB_H
#define SYMTAB_H

#include <iostream>
#include "Item.h"
#include "Heap.h"
using namespace std;

class SymTab : private Heap{
	public:
		// use Insert, Remove, Write, constructor in heap
		using Heap::Insert;
		using Heap::Remove;
		using Heap::Write;
		SymTab (int sz):Heap(sz){}
};

#endif
