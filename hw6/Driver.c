/****************************************************************************

                                                Tianling Luo
                                                CSE 12, winter 2020
                                                Feb 11, 2020
                                                cs12wi20cu
                              Assignment 6

File Name:      Driver.c
Description:    In this assignment, I will write the class implementation
for UCSDStudent which includes all UCSDStudent methods that will be 
called by the HashTable code.
****************************************************************************/

#include <cstdlib>  
#include <cstdio>  
#include <cstring>  
#include <getopt.h>  
#include <iostream>  
#include "Base.h"  
#include "SymTab.h"  
using namespace std;

#ifdef NULL  
#undef NULL  
#define NULL 0  
#endif  

/****************************************************************************
// class UCSDStudent
//
// Description: The UCSDStudent class is defined and it contains the member
// functions that will be called by the Hash.c code.
//
// Data Fields:
//     name (char*)  - the name of UCSD student
//     Studentnum (long)  - the student number of UCSD student
//
// Public functions:
//     UCSDStudent - store the name and student number in name and
//     		     Studentnum with input char* and long
//     ~UCSDStudent - to delete name
//     operator char* - get the name of the student
//     operator long == - is equal function in c++
//     Write - get the name and student number in string
//
****************************************************************************/
class UCSDStudent : public Base {
    char* name;
    long studentnum;
public:
    UCSDStudent(char* nm, long sn) :
        name(strdup(nm)), studentnum(sn) {}
    ~UCSDStudent(void) {
        /* free */
        free(name);
    }

    /* to get the name of student */
    operator char* (void) {
        return name;
    }

    /* isequal function in c++ */
    operator long(void);
    long operator == (Base& base) {
        return !strcmp(name, base);
    }

    /* write element in it as string */
    ostream& Write(ostream& stream) {
        return stream << "name: " << name
            << " Studentnum: " << studentnum;
    }

};

/*---------------------------------------------------------------------------
Function Name:                operator long
Purpose:                      to get the numeric number
Description:                  sum up the ASCII value of each character by
			      iteration
Result:                       get the ASCII value
---------------------------------------------------------------------------*/
UCSDStudent :: operator long (void) {  
    /* initialize the sum */
    long sum = 0;  
    /* use for loop to sum up */
    for (long index = 0; name[index]; index++)  
        sum += name[index];  
  
    return sum;  
}  


int main(int argc, char* const* argv) {
    SymTab ST(5);
    char buffer[128];
    char command;
    long number;
    char option;

    /* initialize debug variable */
    HashTable::Set_Debug(0);

    /* check command line options for debug display */
    while ((option = getopt(argc, argv, "x")) != EOF) {

        switch (option) {
        case 'x': HashTable::Set_Debug(1);
            break;
        }
    }

    ST.Write(cout << "Initial Symbol Table:\n");
    while (cin) {
        command = NULL;     // reset command each time in loop  
        cout << "Please enter a command:  ((i)nsert, (l)ookup, (w)rite):  ";
        cin >> command;

        switch (command) {
        case 'i':
            cout << "Please enter UCSD student name to insert:  ";
            cin >> buffer;    // formatted input  

            cout << "Please enter UCSD student number:  ";
            cin >> number;

            // create student and place in symbol table  
            if (!ST.Insert(new UCSDStudent(buffer, number))) {

                ST.Write(cout << "\nFinal Symbol Table:\n");
                exit(0);
            }
            break;

        case 'l': {
            Base* found;   // whether found or not  

            cout << "Please enter UCSD student name to lookup:  ";
            cin >> buffer;    // formatted input  

            UCSDStudent stu(buffer, 0);
            found = ST.Lookup(&stu);

            if (found)
                found->Write(cout << "Student found!!!\n") << "\n";
            else
                cout << "Student " << buffer << " not there!\n";
        }
                break;

        case 'w':
            ST.Write(cout << "The Symbol Table contains:\n");
        }
    }
    ST.Write(cout << "\nFinal Symbol Table:\n");
}
