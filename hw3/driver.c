/****************************************************************************
Answers to the debugger questions:
1. The address returned by malloc() is 0x604010.
2. The address returned by new_Stack is 0x604028.
3. This address is on the heap.
4. The value of spp is 0x7fffffffd8e8. It is on the heap.
5. The value of *spp is 0x604028. It is on the heap.
6. The value being passed to free is 0x604010.
7. Yes.
****************************************************************************/

#include <stdio.h>  
#include <stdlib.h>  
#include "mylib.h"  
#include "stack.h"  

int main(int argc, char* const* argv) {

    Stack* main_Stack = 0;         /* the test stack */
    unsigned long amount;        /* numbers of items possible go on stack */
    long command;                   /* stack command entered by user */
    long item = 0;                  /* item to go on stack */
    char option;                    /* the command line option */
    long status;                    /* return status of stack functions */

    /* initialize debug states */
    debug_off();

    /* check command line options for debug display */
    while ((option = getopt(argc, argv, "x")) != EOF) {

        switch (option) {
        case 'x': debug_on(); break;
        }
    }

    while (1) {
        command = 0;            /* initialize command, need for loops */
        writeline("\nPlease enter a command:", stdout);
        writeline("\n\t(a)llocate, (d)eallocate, ", stdout);
        writeline("p(u)sh, (p)op, (t)op, (i)sempty, (e)mpty, ", stdout);
        writeline("\n\tis(f)ull, (n)um_elements, (w)rite to stdout, "
            , stdout);
        writeline("(W)rite to stderr.\n", stdout);
        writeline("Please enter choice: ", stdout);
        command = getchar();
        if (command == EOF)     /* are we done? */
            break;
        clrbuf(command);       /* get rid of extra characters */

        switch (command) {      /* process commands */

        /* YOUR CODE GOES HERE */

        case 'a':               /* to allocate a stack */
	    writeline("\nPlease enter the number of objects to be able to store: ",stdout);
	    /* use decin function to store the number */
    	    amount=decin();
	    /* get rid of extra characters */
	    clrbuf(0);
	    
	    /* check if the stack exists */
	    if (main_Stack != NULL)
		    delete_Stack(&main_Stack);

	    /* call new_Stack to allocate a stack */	    
       	    main_Stack = new_Stack(amount);
	    break;

        case 'd':               /* to deallocate a stack */
            delete_Stack(&main_Stack);
	    writeline("Stack has been deleted.\n",stdout);
	    break;

        case 'e':               /* empty stack */
            empty_Stack(main_Stack);
	    writeline("Stack is empty.",stdout);
	    break;

        case 'f':               /* isfull */
            if (isfull_Stack(main_Stack))
                writeline("Stack is full.\n", stdout);
            else
                writeline("Stack is not full.\n", stdout);
            break;

        case 'i':               /* isempty */
            if (isempty_Stack(main_Stack))
                writeline("Stack is empty.\n", stdout);
            else
                writeline("Stack is not empty.\n", stdout);
            break;

        case 'n':               /* get_occupancy */
            writeline("Number of elements on the stack is:  ",
                stdout);
            decout(get_occupancy(main_Stack));
            newline(stdout);
            break;

        case 'p':               /* pop */
            status = pop(main_Stack, &item);
            if (!status)
                fprintf(stderr, "\nWARNING:  pop FAILED\n");
            else {
                writeline(
                    "Number popped from the stack is:  ",
                    stdout);
                decout(item);
                newline(stdout);
            }
            break;

        case 't':               /* top */
            status = top(main_Stack, &item);
            if (!status)
                fprintf(stderr, "\nWARNING:  top FAILED\n");
            else {
                writeline(
                    "Number at top of the stack is:  ",
                    stdout);
                decout(item);
                newline(stdout);
            }
            break;

        case 'u':               /* push */
            writeline(
                "\nPlease enter a number to push to stack:  ",
                stdout);
            item = decin();
            clrbuf(0);     /* get rid of extra characters */
            status = push(main_Stack, item);
            if (!status)
                fprintf(stderr, "\nWARNING:  push FAILED\n");
            break;

        case 'w':               /* write */
            writeline("\nThe Stack contains:\n", stdout);
            write_Stack(main_Stack, stdout);
            break;

        case 'W':               /* write */
            writeline("\nThe Stack contains:\n", stdout);
            write_Stack(main_Stack, stderr);
            break;
        }

        if (item == EOF) /* check if done */
            break;
    }

    if (main_Stack)
        delete_Stack(&main_Stack);     /* deallocate stack */
    newline(stdout);
    return 0;
}
