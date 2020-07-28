//==========================================================================
// cs12wi20cu                         Homework 7              Tianling Luo
//--------------------------------------------------------------------------
// File: Driver.c
//
// Description: In this assignment, we will write the class implementation
// for UCSDStudent which includes all UCSDStudent methods that will be
// called by the Tree code.
//==========================================================================

#include <cstdlib>  
#include <iostream>  
#include <string.h>  
#include "Base.h"  
#include "SymTab.h"  
using namespace std;

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
class UCSDStudent : public Base {
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
    operator const char* (void) const {
        return name;
    }

    // to check if equal
    long operator == (const Base& bbb) const {
        return !strcmp(name, bbb);
    }

    // to check if larger
    long operator > (const Base& bbb) const {
        return (strcmp(name, bbb) > 0) ? 1 : 0;
    }

    // write element in it as string
    ostream& Write(ostream& stream) const {
        return stream << "name:  " << name
            << "  studentnum:  " << studentnum;
    }
};

int main(int argc, char* const* argv) {
    char buffer[80];
    char command;
    long number;

    Tree::Set_Debug(0);

    if (argc != 1 && strcmp("-x", argv[1]) == 0) {
        Tree::Set_Debug(1);
    }

    SymTab* ST;
    ST = new SymTab("UCSDStudentTree");
    ST->Write(cout << "Initial Symbol Table:\n");

    while (cin) {
        command = NULL;     // reset command each time in loop  
        cout << "Please enter a command:  ((a)llocate, (i)nsert, " <<
            "(l)ookup, (r)emove, (w)rite):  ";
        cin >> command;

        switch (command) {

        case 'a':
            cout << "Please enter name of new Tree to allocate:  ";
            cin >> buffer;

            delete ST;
            ST = new SymTab(buffer);

            break;

        case 'i':
            cout << "Please enter UCSD student name to insert:  ";
            cin >> buffer;    // formatted input  

            cout << "Please enter UCSD student number:  ";
            cin >> number;

            // create student and place in symbol table  
            ST->Insert(new UCSDStudent(buffer, number));
            break;

        case 'l': {
            const Base* found; // whether found or not  

            cout << "Please enter UCSD student name to lookup:  ";
            cin >> buffer;    // formatted input  

            UCSDStudent stu(buffer, 0);
            found = ST->Lookup(&stu);

            if (found)
                found->Write(cout << "Student found!\n") << "\n";
            else
                cout << "student " << buffer << " not there!\n";
        }
                break;

        case 'r': {
            Base* removed; // data to be removed  

            cout << "Please enter UCSD student name to remove:  ";
            cin >> buffer;    // formatted input  

            UCSDStudent stu(buffer, 0);
            removed = ST->Remove(&stu);

            if (removed)
                removed->Write(cout << "Student removed!\n") << "\n";
            else
                cout << "student " << buffer << " not there!\n";
        }
                break;


        case 'w':
            ST->Write(cout << "The Symbol Table contains:\n");
        }
    }

    ST->Write(cout << "\nFinal Symbol Table:\n");
    delete ST;
}
