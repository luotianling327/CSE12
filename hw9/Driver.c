//==========================================================================
// cs12wi20cu                         Homework 9              Tianling Luo
//--------------------------------------------------------------------------
// File: Driver.c
//
// Description: In this assignment, Driver.c will be used to test the Tree
// data structure.
//==========================================================================

#include <iostream>
#include <fstream>
#include <cstdio>
#include <string>
#include <getopt.h>
#include "Driver.h"
#include "SymTab.h"

using namespace std;

/* DO NOT CHANGE:  This file is used in evaluation */

#ifdef NULL
#undef NULL
#define NULL 0
#endif


ostream& operator << (ostream& stream, const UCSDStudent& stu) {
    return stream << "name:  " << stu.name
        << " with studentnum:  " << stu.studentnum;
}

//==========================================================================
// Routine Name : main
// File :         Driver.c
//
// Description : to set command options
//==========================================================================
int main(int argc, char* const* argv) {
    char buffer[BUFSIZ];
    char command;
    long number;
    char option;

	// declare istream pointer
	istream* is = &cin;
	ostream* os = &cout;

    SymTab<UCSDStudent>::Set_Debug_Off();

    while ((option = getopt(argc, argv, "x")) != EOF) {

        switch (option) {
        case 'x': SymTab<UCSDStudent>::Set_Debug_On();
            break;
        }
    }

    SymTab<UCSDStudent> ST("Driver.datafile");
    ST.Write(*os << "Initial Symbol Table:\n");

    while (cin) {
        // check for EOF
        if (!*is && is != &cin) {
            delete is;
	    delete os;
            // poingt back to cin/cout again
            is = &cin; // read from keyboard
            os = &cout;
        } 
        command = NULL;         // reset command each time in loop
        *os << "Please enter a command ((f)ile, (i)nsert, "
            << "(l)ookup, (r)emove, (w)rite):  ";
        *is >> command;

        switch (command) {
        case 'i': {
            *os << "Please enter UCSD student name to insert:  ";
            *is >> buffer;  // formatted input

            *os << "Please enter UCSD student number:  ";
            *is >> number;

            UCSDStudent stu(buffer, number);

            // create student and place in symbol table
            ST.Insert(stu);
            break;
        }
        case 'l': {
            unsigned long found;    // whether found or not

            *os << "Please enter UCSD student name to lookup:  ";
            *is >> buffer;  // formatted input

            UCSDStudent stu(buffer, 0);
            found = ST.Lookup(stu);

            if (found)
                cout << "Student found!!!\n" << stu << "\n";
            else
                cout << "student " << buffer << " not there!\n";
            break;
        }
        case 'r': {
            unsigned long removed;

            *os << "Please enter UCSD student name to remove:  ";
            *is >> buffer;  // formatted input

            UCSDStudent stu(buffer, 0);
            removed = ST.Remove(stu);

            if (removed)
                cout << "Student removed!!!\n" << stu << "\n";
            else
                cout << "student " << buffer << " not there!\n";
            break;
        }
	// for command 'f'
        case 'f': {
            // to get the file name
            *os << "Please enter file name for commands:  ";
            *is >> buffer;
            // if pointers don't currently point to cin
            if (is != &cin) {
		// delete is and reset
                delete is;
                is = new ifstream(buffer);
		// delete os and reset
                delete os;
                os = new ofstream("/devl/null");
            }
	    else{
		is=new ifstream(buffer);
		os=new ofstream("devl/null");
	    }
            break;
        }
        case 'w': {
            ST.Write(cout << "The Symbol Table contains:\n");
	}
        }   
    }
    ST.Write(cout << "\nFinal Symbol Table:\n");
    // prints out cost and operation information
    if (ST.GetOperation() != 0) {
        cout << "\nCost of operations: ";
        cout << ST.GetCost();
        cout << " tree accesses";

        cout << "\nNumber of operations: ";
        cout << ST.GetOperation();

        cout << "\nAverage cost: ";
        cout << (((float)(ST.GetCost())) / (ST.GetOperation()));
        cout << " tree accesses/operation\n";
    }

    else
        cout << "\nNo cost information available.\n";
    // end of the code added

}
