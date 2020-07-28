//==========================================================================
// cs12wi20cu                         Homework 8              Tianling Luo
//--------------------------------------------------------------------------
// File: Driver.h
//
// Description: In this assignment, we will complete the definition for
// the UCSDStudent class in Driver.h.
//==========================================================================
#ifndef DRIVER_H
#define DRIVER_H

#include <string.h>
#include <iostream>
#include <cstdlib>
using namespace std;

//=========================================================================
// class UCSDStudent
//
// Description: The UCSDStudent class is defined and it contains the member
// functions that will be called by the Tree.c code.
//
// Data Fields:
//     operator (friend ostream &) - the operator
//     name (char[]) - the name of UCSD student
//     studentnum (long) - the student number of UCSD student
//
// Public functions:
//     UCSDStudent - constructor
//     operator const char* - get the name of the student
//     operator long == - is equal function in c++
//     operator long > - isGreaterThan function in c++
//==========================================================================  
class UCSDStudent {
        friend ostream & operator << (ostream &, const UCSDStudent &);
        char name[20];
        long studentnum;
public:

//==========================================================================
// Name :         UCSDStudent
// File :         Driver.h
//
// Description :  constructor of UCSDStudent
//==========================================================================
    UCSDStudent(char* nm, long sn) :
        studentnum(sn) {
		// constructor
		strcpy(name,nm);
	}

//==========================================================================
// Name :         operator const char*
// File :         Driver.h
//
// Description :  get the name of the student
//==========================================================================
        operator const char* (void) const {
	    // return the name
            return name;
        }

//==========================================================================
// Name :         operator long ==
// File :         Driver.h
//
// Description :  to check if equal
//==========================================================================
        long operator == (const UCSDStudent& bbb) const {
	    // == operator
            return !strcmp(name, bbb.name);
        }

//==========================================================================
// Name :         operator long >
// File :         Driver.h
//
// Description :  to check if is larger
//==========================================================================
        long operator > (const UCSDStudent& bbb) const {
	    // > operator
            return (strcmp(name, bbb.name) > 0) ? 1 : 0;
        }

};

#endif

