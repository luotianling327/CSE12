/****************************************************************************

                                                Tianling Luo
                                                CSE 12, winter 2020
                                                Feb 11, 2020
                                                cs12wi20cu
                              Assignment 6

File Name:      Hash.c
Description:    In the assignment, I will implement a Symbol Table to
allow variable assignment to the calculator. 
****************************************************************************/

#include <cstdlib>  
#include <string>  
#include "Hash.h"  

using namespace std;

static const char AND[] = " and ";
static const char DEBUG_ALLOCATE[] = " - Allocated]\n";
static const char DEBUG_DEALLOCATE[] = " has been deallocated]\n";
static const char DEBUG_LOCATE[] = " - Locate]\n";
static const char DEBUG_LOOKUP[] = " - Lookup]\n";
static const char BUMP[] = "[Bumping To Next Location...]\n";
static const char COMPARE[] = " - Comparing ";
static const char FOUND_SPOT[] = " - Found Empty Spot]\n";
static const char HASH[] = "[Hash Table ";
static const char HASH_VAL[] = "[Hash value is: ";
static const char INSERT[] = " - Inserting ";
static const char PROCESSING[] = "[Processing ";
static const char TRYING[] = "[Trying index ";

bool HashTable::debug = 0;
int HashTable::counter = 0;

void HashTable::Set_Debug(int option)
/***************************************************************************
% Routine Name : HashTable :: Set_Debug (public)
% File :         Hash.c
%
% Description :  This function sets debug mode on or off
%
% Parameters descriptions :
%
% name               description
% ------------------ ------------------------------------------------------
% option             true should set debug mode on, false should set debug
%                    mode off.
***************************************************************************/

{
    /* set debug as the option */
    debug = option;
    return;
}


HashTable::HashTable(int sz) : size(sz),
table_count(++counter), occupancy(0), table(new Base* [sz]),
probeCount(new int[sz])
/***************************************************************************
% Routine Name : HashTable :: HashTable (public)
% File :         Hash.c
%
% Description :  This function allocates and initializes the memory
%                associated with a hash table.
%
% Parameters descriptions :
%
% name               description
% ------------------ ------------------------------------------------------
% size               The number of elements for the table...MUST BE PRIME!!!
***************************************************************************/

{
    /* use in the loop as counter */
    int iteration;
    /* use loop to intialize to null */
    for (iteration = 0; iteration < size; iteration++) {
        table[iteration] = NULL;
    }

    //for debugging message
    if (debug) {
        cerr << HASH << table_count << DEBUG_ALLOCATE;
    }

}   /* end: HashTable */


HashTable :: ~HashTable(void)
/***************************************************************************
% Routine Name : HashTable :: ~HashTable  (public)
% File :         Hash.c
%
% Description :  deallocates memory associated with the Hash Table.  It
%                will also delete all the memory of the elements within
%                the table.
***************************************************************************/

{
    /* use iteration to do iteration */
    int iteration;
    for (iteration = 0; iteration < size; iteration++) {
        delete table[iteration];
	table[iteration]=NULL;
    }
        /* delete table itself */
        delete[] table;
	delete[] probeCount;
        /* set to null */
        table = NULL;
	probeCount=NULL;

	/* for debugging message */
	if(debug){
		cerr << HASH << table_count << DEBUG_DEALLOCATE;
	}
}   /* end: ~HashTable */


bool HashTable::Insert(Base* element, int initialCount)
/***************************************************************************
% Routine Name : HashTable :: Insert (public)
% File :         Hash.c
%
% Description : This function will insert the element in the hash table.
%       If the element cannot be inserted, false will be returned.
%       If the element can be inserted, the element is inserted
%       and true is returned.  Duplicate insertions will
%       cause the existing element to be deleted, and the duplicate
%       element to take its place.
%
% Parameters descriptions :
%
% name               description
% ------------------ ------------------------------------------------------
% element            The element to insert.
% initialCount       Where to start in probe seq (recursive calls)
% <return>           true or false indicating success or failure of insertion
***************************************************************************/

{
    /* use temp to store the original element at index */
    Base* temp;
    /* use temp_count to store the probe seq */
    int temp_count;
    /* set count field */
    count = initialCount;
    /* use locate to store the Locate return */
    bool locate;

    /* for debugging message */
    if (debug) {
        cerr << HASH << table_count << INSERT
            << (char*) (*element) << "]" << endl;
    }

    /* store the return of locate */
    locate = Locate(element);

    /* if locate returns true */
    if (locate){
	table[index]=element;
	probeCount[index]=count;
	if (occupancy<size)
		occupancy++;
        return true;
    }
    /* if the table is full */
    if (occupancy >= size) {
        return false;
    }

    /* if locate returns false */
    if (!locate) {
        /* if it's empty at index */
        if (table[index] == NULL) {
            /* increment the occupancy */
            occupancy++;
            table[index] = element;
            probeCount[index] = count;
            return true;
        }
        /* if there already exists an item */
        else {
            temp = table[index];
            temp_count = probeCount[index];
            table[index] = element;
	    probeCount[index]=count;

            /* for debugging message */
            if (debug) {
                cerr << BUMP;
            }

            /* use recursion to bump */
            Insert(temp, temp_count + 1);

            return true;
        }
    }
    return false;
}


bool HashTable::Locate(Base* element)
/**********************************************************************
% Routine Name : HashTable :: Locate (private)
% File :         HashTable.c
%
% Description : This function will locate the location in the
%               table for the insert or lookup.
%
% Parameters descriptions :
%
% name               description
% ------------------ -------------------------------------------------
% element            The element needing a location.
% <return>           true if tem found, or false if not found
**********************************************************************/

{
    /* get the numeric arrtibute */
    int ASCII_Sum = (long)(*element);
    /* initial location */
    int Init = ASCII_Sum % size;
    /* increment */
    int increment = (ASCII_Sum % (size - 1) + 1);
    /* use index to store current location */
    index = (Init+(count-1)*increment)%size;

    if (debug) {
        cerr << HASH << table_count << DEBUG_LOCATE;
        cerr << PROCESSING << (char*) (*element) << "]" << endl;
        cerr << HASH_VAL << ASCII_Sum << "]" << endl;
    }

    /* use loop to find the item */
    while (1) {
        /* for debugging message */
        if (debug) {
            cerr << TRYING << index << "]" << endl;
        }

        /* if finds an empty index */
        if (table[index] == NULL) {
            /* for debug message */
            if (debug) {
                cerr << HASH << table_count << FOUND_SPOT;
            }
            return false;
        }

        /* for debug message */
        if (debug) {
            cerr << HASH << table_count << COMPARE
                << (char*) (*element) << AND
                << (char*) (*table[index]) << "]" << endl;
        }

        /* if the element is found */
        if (*table[index]==*element)
            return true;

        /* if the occupied index hasn't searched */
        /* as long as the current element has */
        /* searched */
        if (probeCount[index] < count)
            return false;

        /* if we exhaust the table */
        if (count > size)
            return false;

        /* let index be next location index */
        index = (index + increment) % size;
        /* increment count */
        count++;
    }

    return false;
}

Base* HashTable::Lookup(Base* element)
/***************************************************************************
% Routine Name : HashTable :: Lookup (public)
% File :         Hash.c
%
% Description : This function will lookup the element in the hash table.  If
%               found a pointer to the element is returned.  If the element
%               is not found, NULL will be returned to the user.
%
% Parameters descriptions :
%
% name               description
% ------------------ ------------------------------------------------------
% element            The element to lookup.
% <return>           A pointer to the element if found, else NULL
***************************************************************************/

{
    /* set count */
    count = 1;
    /* use locate to store Locate return */
    bool locate;

    /* for debugging message */
    if (debug) {
        cerr << HASH << table_count << DEBUG_LOOKUP;
    }

    /* store the return of locate */
    locate = Locate(element);
    /* if locate returns true */
    if (locate) {
        return table[index];
    }
    /* if locate returns false */
    return 0;
}


ostream& HashTable::Write(ostream& stream)
/***************************************************************************
% Routine Name : HashTable :: Write (public)
% File :         Hash.c
%
% Description : This funtion will output the contents of the hash table
%               to the stream specificed by the caller.  The stream could be
%               cerr, cout, or any other valid stream.
%
% Parameters descriptions :
%
% name               description
% ------------------ ------------------------------------------------------
% stream             A reference to the output stream.
% <return>           A reference to the output stream.
***************************************************************************/

{
    stream << "Hash Table " << table_count << ":\n"
        << "size is " << size << " elements, "
        << "occupancy is " << occupancy << " elements.\n";

    /* go through all table elements */
    for (int index = 0; index < size; index++)
        if (table[index])
            table[index]->Write(stream << "at index "
                << index << ": ") << " with probeCount: "
            << probeCount[index] << "\n";
    return stream;
}   /* end: Write */
