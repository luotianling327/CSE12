//==========================================================================
// cs12wi20cu                         Homework 10              Tianling Luo
//--------------------------------------------------------------------------
// File: Item.h
//
// Description: In this assignment, we will implement a heap data
// structure to use the "template" feature of C++. 
//==========================================================================

#ifndef ITEM_H
#define ITEM_H

#include <iostream>
using namespace std;

struct Item{
	// get char
	virtual operator char* (void){
		return 0;
	}
	// is equal function
	virtual long operator == (Item & item){
		return *this == item;
	}
	// greater than
	virtual long operator > (Item & item){
		return 0;
	}
	Item (void){}//new item
	virtual ~Item(void){}//delete item
	virtual ostream & Write(ostream &)=0;// wtite items
};

#endif
