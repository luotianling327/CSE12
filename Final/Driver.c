//==========================================================================
// cs12wi20cu                         Homework 10              Tianling Luo
//--------------------------------------------------------------------------
// File: Driver.c
//
// Description: In this assignment, Driver.c will be used to test the heap
// data structure.
//==========================================================================

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <getopt.h>
#include "Item.h"
#include "SymTab.h"

using namespace std;

/* DO NOT CHANGE:  This file is used in evaluation */

#ifdef NULL
#undef NULL
#define NULL 0
#endif

//=========================================================================
// class UCSDStudent
//
// Description: The UCSDStudent class is defined and it contains the member
// functions that will be called by the Tree.c code.
//
// Data Fields:
//     name (char*)  - the name of UCSD student
//     studentnum (long)  - the student number of UCSD student
//
// Public functions:
//     UCSDStudent - store the name and student number in name and
//     		     Studentnum with input char* and long
//     ~UCSDStudent - to delete name
//     operator const char* - get the name of the student
//     operator long == - is equal function in c++
//     operator long > - isGreaterThan function in c++
//     Write - get the name and student number in string
//==========================================================================  
class UCSDStudent :public Item{
    char* name;
    long studentnum;
public:
    // constructor 
    UCSDStudent(char* nm, long sn = 0) :
        name(strdup(nm)), studentnum(sn) {}

    // destructor
    ~UCSDStudent(void) {
        // freeing
        free(name);
    }
    // to get the name of student
    operator char* (void) {
        return name;
    }
    // to check if equal
    long operator == (Item& bbb){
        return !strcmp(name, bbb);
    }
    // to check if larger
    long operator > (Item& bbb) {
        return (strcmp(name, bbb) > 0) ? 1 : 0;
    }
    // write element in it as string
    ostream& Write(ostream& stream) {
        return stream << "name:  " << name
            << "  studentnum:  " << studentnum;
    }
};

int main (int argc, char * const * argv) {
        char buffer[BUFSIZ];
        char command;
        long number;
        char option;
	int size;

	cout << "Please enter the number of objects "
			<<"to be able to store: ";
	cin >> size;
        SymTab ST(size);
	// initialize debug variable
	Heap::Set_Debug_Off ();

        while ((option = getopt (argc, argv, "x")) != EOF) {
        switch (option) {
		case 'x': Heap::Set_Debug_On ();
                        break;
                }       
        }
	ST.Write (cout << "The Initial Heap:\n" );

        while (cin) {
                command = NULL;         // reset command each time in loop
		cout << "Please enter a command ((i)nsert, "
                        << "(r)emove, (w)rite):  ";
                cin >> command;

                switch (command) {
                case 'i': {
                        cout << "Please enter UCSD student name to insert:  ";
                        cin >> buffer;  // formatted input

                        cout << "Please enter UCSD student number:  ";
                        cin >> number;

			// create student and place in symbol table
			if (!ST.Insert(new UCSDStudent(buffer, number))){
				cout<<"Heap is full, could not insert!!\n";
			}			
			break;
                }
                case 'r': { 
                        Item* removed;			
                        removed = ST.Remove();

                        if (removed!=NULL)
                                removed->Write(cout<<"Student removed!!!\n")
					<< "\n";
                        else
                                cout << "Heap is empty!\n";
                        break;
                }
                case 'w':
                        ST.Write (cout << "The Heap contains:\n");
                }
        }

        ST.Write (cout << "\nFinal Heap contains:\n");
}
