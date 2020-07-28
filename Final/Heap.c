//==========================================================================
// cs12wi20cu                         Homework 10              Tianling Luo
//--------------------------------------------------------------------------
// File: Heap.c
//
// Description: In this assignment, we will implement a heap data
// structure in C++.
//==========================================================================
#include <cstdlib>
#include <string>
#include "Heap.h"
using namespace std;

//messages
static const char AND[] = " and ";
static const char SWAP[]= "[ SWAP ";
static const char HEAP[] = "[ HEAP";
static const char ALLOCATE[] = " - Allocating]\n";
static const char DEALLOCATE[] = " - Deallocating]\n";
static const char INSERT[] = " - Inserting ";
// for debug option
int Heap::debug = 0;
#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

#define TWO 2

//==========================================================================
// Routine Name : Heap :: Set_Debug_On
// File :         Heap.c
//
// Description :  Set debug on
//==========================================================================
void Heap::Set_Debug_On(void)
{
    // set the debug on
    debug = 1;
}
//==========================================================================
// Routine Name : Heap :: Set_Debug_Off
// File :         Heap.c
//
// Description :  Set debug off
//==========================================================================
void Heap::Set_Debug_Off(void)
{
    // set the debug off
    debug = 0;
}

//==========================================================================
// Routine Name : Heap :: Heap
// File :         Heap.c
//
// Description : Constructor of Hea that initialize arr[] and occupancy.
//==========================================================================
Heap ::Heap(int sz) : occupancy(0), size(sz)
{
    // for debug message
    if (debug) {
        cerr << HEAP << ALLOCATE;
    }
    // initialize arr
    arr=new Item*[sz];
    for (int iteration=0;iteration<size;iteration++)
	    arr[iteration]=NULL;
}
//==========================================================================
// Routine Name : Heap :: ~Heap
// File :         Heap.c
//
// Description :  deallocates memory associated with the Heap.
//==========================================================================
Heap :: ~Heap(void)
{
    // for debug message
    if (debug) {
        cerr << HEAP << DEALLOCATE;
    }
    // delete each element and set to null
    for (int iteration=0;iteration<size;iteration++){
	    delete arr[iteration];
	    arr[iteration]=NULL;
    }
    // delete arr
    delete[] arr;
    arr=NULL;
}

//==========================================================================
// Routine Name : Heap ::swapItem
// File :         Heap.c
//
// Description : swap two items in heap.
//
// Parameters descriptions :
//
// name               description
// ------------------ ------------------------------------------------------
// index1             the first index of item to swap
// index2             the second index of item to swap
// <return>           true or false indicating success of insertion.
//==========================================================================
void Heap ::swapItem(int index1,int index2) {
	// use temp for swapping
	Item* temp;
	// swap
	temp=arr[index1];
	arr[index1]=arr[index2];
	arr[index2]=temp;
	// for debug message
        if (debug) {
            cerr << SWAP << (char*)(*arr[index1]) << AND << 
                (char*)(*arr[index2]) << "]" << endl;
        }
}

//==========================================================================
// Routine Name : Heap ::reheapUp
// File :         Heap.c
//
// Description : Called in Insert function to sort the heap.
//
// Parameters descriptions :
//
// name               description
// ------------------ ------------------------------------------------------
// element            the data to be inserted.
// <return>           true or false indicating success of insertion.
//==========================================================================
void Heap ::reheapUp(int current) {
	if(current>0){
		int parent=(current-1)/TWO;
		if(*arr[parent]>*arr[current]){
			// swap the item with its parent
			swapItem(parent,current);
			// call recursively
			reheapUp(parent);
		}
	}
	return;
}

//==========================================================================
// Routine Name : Heap ::Insert
// File :         Heap.c
//
// Description : Inserts an element into the heap.
//
// Parameters descriptions :
//
// name               description
// ------------------ ------------------------------------------------------
// element            the data to be inserted.
// <return>           true or false indicating success of insertion.
//==========================================================================
unsigned long Heap ::Insert(Item* element) {
	if (occupancy<size){
		// for debug message
        	if (debug) {
            		cerr << HEAP << INSERT << 
                	(char*)(*element) << "]" << endl;
		}
		// insert at the end of heap
		arr[occupancy]=element;
		// reheap up the tree
		reheapUp(occupancy);
		// increment on occupancy
		occupancy++;
		return 1;
	}
	return 0;
}

//==========================================================================
// Routine Name : Heap ::reheapDown
// File :         Heap.c
//
// Description : Called in Remove function to maintain the structure of heap
//
// Parameters descriptions :
//
// name               description
// ------------------ ------------------------------------------------------
// current            the index of current item.
// <return>           true or false indicating success of insertion.
//==========================================================================
void Heap ::reheapDown(int current) {
	// index of left child
	int left=current*TWO+1;
	// index of right child
	int right=current*TWO+TWO;
	// if occupancy greater than right
	if(right<=occupancy){
		// find the max child
		int min=(*arr[left]>*arr[right])?
			right:left;
		if (*arr[current]>*arr[min]){
			// swap
			swapItem(current,min);
			// call recursively
			reheapDown(min);
		}
	}
	// if occupancy smaller than right but larger than left
	else if(left<=current){
		// find the max child
		int min=left;
		if (*arr[current]>*arr[min]){
			// swap
			swapItem(current,min);
			// call recursively
			reheapDown(min);
		}
	}
	return;
}

//==========================================================================
// Routine Name : Heap ::Remove
// File :         Heap.c
//
// Description : Removes an element from the Heap.
// 
// Parameters descriptions :
//
// name               description
// ------------------ ------------------------------------------------------
// element            the data to be removed
// <return>           Returns true or false indicating success of removal
//==========================================================================
Item* Heap ::Remove(void) {
	if (occupancy==0){
		return NULL;
	}
	Item *root=arr[0];
	// replace the root with the last item
	arr[0]=arr[occupancy-1];
	// decrement on occupancy
	occupancy--;
	// reheap down the tree
	reheapDown(0);
	return root;
}

//==========================================================================
// Routine Name : Heap :: Write
// File :         Heap.c
//
// Description : This function will output the contents of the Heap
//               to the stream specificed by the caller.  The stream could be
//               cerr, cout, or any other valid stream.
//
// Parameters descriptions :
//
// name               description
// ------------------ ------------------------------------------------------
// stream             A reference to the output stream.
// <return>           A reference to the output stream.
//==========================================================================
ostream& Heap ::Write(ostream& stream)
{
    stream << "The heap has " << occupancy << " items.\n";
    for (int iteration=0 ;iteration < occupancy;iteration++){
	    // write out the heap items
	    arr[iteration]->Write(stream << "At index " 
			    << iteration<< ":  ")<<"\n";
    }
    return stream;
}
