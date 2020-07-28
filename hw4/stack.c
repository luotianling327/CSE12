/**************************************************************************** 
 
                                                        Tianling Luo 
                                                        CSE12, winter 
                                                        Jan 27, 2020
                                                        cs12wi20cu
                                Assignment Four 
 
File Name:      stack.c
Description:    This program contains stack operations.
****************************************************************************/

#include <malloc.h>  
#include <stdio.h>  
#include "mylib.h"  
#include "stack.h"  

#define STACK_POINTER_INDEX (-1)        /* Index of next available space */  
#define STACK_SIZE_INDEX (-2)           /* Index of size of the stack */  
#define STACK_COUNT_INDEX (-3)          /* Index of which stack allocated */  
#define STACK_OFFSET 3  /* offset from allocation to where user info begins */  

/* catastrophic error messages */
static const char DELETE_NONEXIST[] = "Deleting a non-existent stack!!!\n";
static const char EMPTY_NONEXIST[] = "Emptying a non-existent stack!!!\n";
static const char INCOMING_NONEXIST[] =
"Incoming parameter does not exist!!!\n";
static const char ISEMPTY_NONEXIST[] =
"Isempty check from a non-existent stack!!!\n";
static const char ISFULL_NONEXIST[] =
"Isfull check from a non-existent stack!!!\n";
static const char NUM_NONEXIST[] =
"get_occupancy check from a non-existent stack!!!\n";
static const char POP_NONEXIST[] = "Popping from a non-existent stack!!!\n";
static const char POP_EMPTY[] = "Popping from an empty stack!!!\n";
static const char PUSH_NONEXIST[] = "Pushing to a non-existent stack!!!\n";
static const char PUSH_FULL[] = "Pushing to a full stack!!!\n";
static const char TOP_NONEXIST[] = "Topping from a non-existent stack!!!\n";
static const char TOP_EMPTY[] = "Topping from an empty stack!!!\n";
static const char WRITE_NONEXIST_FILE[] =
"Attempt to write using non-existent file pointer!!!\n";
static const char WRITE_NONEXIST_STACK[] =
"Attempt to write to a non-existent stack!!!\n";

/* Debug messages */
static const char ALLOCATED[] = "[Stack %ld has been allocated]\n";
static const char DEALLOCATE[] = "[Stack %ld has been deallocated]\n";
static const char HEXPOP[] = "[Stack %ld - Popping 0x%lx]\n";
static const char HEXPUSH[] = "[Stack %ld - Pushing 0x%lx]\n";
static const char HEXTOP[] = "[Stack %ld - Topping 0x%lx]\n";
static const char POP[] = "[Stack %ld - Popping %ld]\n";
static const char PUSH[] = "[Stack %ld - Pushing %ld]\n";
static const char TOP[] = "[Stack %ld - Topping %ld]\n";

/* static variable allocation */
static int debug = FALSE; /* allocation of debug flag */
static int stack_counter = 0; /* number of stacks allocated so far */

/* Debug state methods */
void debug_off(void) {
    debug = FALSE;
}

void debug_on(void) {
    debug = TRUE;
}

/*--------------------------------------------------------------------------
Function Name:         delete_Stack
Purpose:               This destructor function deallocates all memory
                       associated with the stack and sets its pointer in
                       the calling function to NULL. 
Description:           First, check that the parameter of spp is not a
                       null pointer and that the pointer it's pointing
                       to is not a null pointer. If so, print out the error
                       message to stderr. If a stack exists, then use the
                       method free to deallocate the memory.
Input:                 spp: double pointer to the stack to be deleted.
Result:                Returns TRUE upon success, FALSE upon failure.
--------------------------------------------------------------------------*/
/* start of true stack code */
long delete_Stack(Stack** spp) {
    /* first, check spp is not a null pointer
     * and the pointer it's pointing to is not a null pointer */
    if (spp == NULL || *spp == NULL) {
        fprintf(stderr, DELETE_NONEXIST);
        return 0;
    }

    /* use free to deallocate memory */
    free(*spp - STACK_OFFSET);
    /* set *spp to null */
    *spp = NULL;
    
    /* for debug messages */
    if (debug == 1) {
        fprintf(stderr, DEALLOCATE, (long)stack_counter);
    }
    stack_counter--;
    return 1;
}

/*--------------------------------------------------------------------------
Function Name:         empty_Stack
Purpose:               Effectively empties the stack of all its elements.
Description:           Reset the STACK_POINTER_INDEX to be 0.
Input:                 this_Stack: pointer to the stack to be emptied.
Result:                The stack becomes empty. No return value.
--------------------------------------------------------------------------*/
void empty_Stack(Stack* this_Stack) {
    /* to check if the stack exists */
    if (this_Stack == NULL) {
        fprintf(stderr, EMPTY_NONEXIST);
        return;
    }

    this_Stack[STACK_POINTER_INDEX] = 0;
    return;
}

/*--------------------------------------------------------------------------
Function Name:         isempty_Stack
Purpose:               Check if a stack is empty or not.
Description:           Check emptiness by checking the stack pointer index.
Input:                 this_Stack: pointer to the stack to be checked.
Result:                Returns true if this_Stack is empty, and false if
                       it is not. 
--------------------------------------------------------------------------*/
long isempty_Stack(Stack* this_Stack) {
    /* to check if the stack exists */
    if (this_Stack == NULL) {
        fprintf(stderr, ISEMPTY_NONEXIST);
        return 0;
    }

    /* to check if the stack is empty*/
    if (this_Stack[STACK_POINTER_INDEX] == 0)
        return 1;
    else
        return 0;
}

/*--------------------------------------------------------------------------
Function Name:         isfull_Stack
Purpose:               Check if a stack is full or not.
Description:           First, check the pointer being passes on. Then,
                       compare STACK_POINTER_INDEX and STACK_SIZE_INDEX.
Input:                 this_Stack: pointer to the stack to be checked.
Result:                Returns true if it is full, and false otherwise. 
--------------------------------------------------------------------------*/
long isfull_Stack(Stack* this_Stack) {
    /* to check if the stack exists */
    if (this_Stack == NULL) {
        fprintf(stderr, ISFULL_NONEXIST);
        return 0;
    }

    /* to check if the stack is full */
    if (this_Stack[STACK_POINTER_INDEX] >= this_Stack[STACK_SIZE_INDEX])
        return 1;

    /* if the stack is not full yet */
    else
        return 0;
}

/*--------------------------------------------------------------------------
Function Name:         new_Stack
Purpose:               This constructor function allocates and initializes
                       a new Stack object.
Description:           Use malloc to allocate memory for the stack.
Input:                 stacksize: the number of longs to hold
Result:                returns a pointer to the first storage space in
                       the stack. 
--------------------------------------------------------------------------*/
Stack* new_Stack(unsigned long stacksize) {
    /* allocate */
    void* memory = malloc((STACK_OFFSET + stacksize) * sizeof(long));
    Stack* this_Stack = (Stack *) memory + STACK_OFFSET;
    /* initialize */
    this_Stack[STACK_POINTER_INDEX] = 0;
    this_Stack[STACK_SIZE_INDEX] = stacksize;
    stack_counter++;
    this_Stack[STACK_COUNT_INDEX] = stack_counter;

    if (debug == 1) {
        fprintf(stderr, ALLOCATED, (long)stack_counter);
    }

    return this_Stack;
}

/*--------------------------------------------------------------------------
Function Name:         get_occupancy
Purpose:               Return the number of elements in the stack.
Description:           Find the number by checking the stack pointer index.
Input:                 this_Stack: a pointer to the stack to be checked.
Result:                Returns the number of elements in this_Stack.
--------------------------------------------------------------------------*/
long get_occupancy(Stack* this_Stack) {
    /* to check if the stack exists */
    if (this_Stack == NULL) {
        fprintf(stderr, NUM_NONEXIST);
        return 0;
    }

    return this_Stack[STACK_POINTER_INDEX];
}

/*--------------------------------------------------------------------------
Function Name:         pop
Purpose:               Removes an item from the top of the stack, and sends
                       it back through the output parameter item. 
Description:           First, do error checking. Set the int being pointed
                       to by item to whatever value is going to be popped.
                       Lastly, decrement the value in the stack at the
                       STACK_POINTER_INDEX.
Input:                 this_Stack: a pointer to the stack to be checked.
                       item: a pointer to a long which will store the
                       popped item.
Result:                Returns true upon success and false upon failure.
--------------------------------------------------------------------------*/
long pop(Stack* this_Stack, long* item) {
    /* use to store stack pointer */   
    long stack_pointer;

    /* to check if the stack exists */
    if (this_Stack == NULL) {
        fprintf(stderr, POP_NONEXIST);
        return 0;
    }

    /* to check the item exists */
    if (item == NULL) {
        fprintf(stderr, INCOMING_NONEXIST);
        return 0;
    }

    /* to check if the stack is empty */
    if (isempty_Stack(this_Stack) == 1) {
        fprintf(stderr, POP_EMPTY);
        return 0;
    }
    
    /* store the stack pointer of the stack in long */
    stack_pointer = this_Stack[STACK_POINTER_INDEX]-1;
    /* to set the int pointed to by item */
    *item = this_Stack[stack_pointer];
    /* to decrement the value */
    this_Stack[STACK_POINTER_INDEX]--;

    /* to print out debug messages */
    if (debug == 1) {
	    if(*item>=0){
		    fprintf(stderr, POP, (long)this_Stack[STACK_COUNT_INDEX], *item);
	    }
	    else{
		    fprintf(stderr, HEXPOP, (long)this_Stack[STACK_COUNT_INDEX], *item);
	    }
    }

    return 1;
}

/*--------------------------------------------------------------------------
Function Name:         push
Purpose:               Adds item to the top of this_Stack. 
Description:           First, do error checking. Use STACK_POINTER_INDEX
                       to find the next available space in stack and store
                       the number. At last, change the value of stack
                       pointer index.
Input:                 this_Stack: a pointer to the stack to be checked.
                       item: a long to be pushed on the stack.
Result:                Returns true upon success and false upon failure.
--------------------------------------------------------------------------*/
long push(Stack* this_Stack, long item) {
    /* use to store stack pointer */   
    long stack_pointer;

    /* to check if the stack exists */
    if (this_Stack == NULL) {
        fprintf(stderr, PUSH_NONEXIST);
        return 0;
    }

    /* to check the item exists */
    if (item == NULL) {
        fprintf(stderr, INCOMING_NONEXIST);
        return 0;
    }

    /* to check if the stack is full */
    if (isfull_Stack(this_Stack) == 1) {
        fprintf(stderr, PUSH_FULL);
        return 0;
    }

    /* store the stack pointer of the stack in long */
    stack_pointer = this_Stack[STACK_POINTER_INDEX];
    /* place the item onto this stack */
    this_Stack[stack_pointer] = item;
    /* to increment the value */
    this_Stack[STACK_POINTER_INDEX]++;

    /* for debug messages */
    if (debug == 1) {
            if(item>=0){
		    fprintf(stderr, PUSH, (long)this_Stack[STACK_COUNT_INDEX], item);
	    }
	    else{
		    fprintf(stderr, HEXPUSH, (long)this_Stack[STACK_COUNT_INDEX], item);
	    }
    }

    return 1;
}

/*--------------------------------------------------------------------------
Function Name:         top
Purpose:               Sends back the item on the top of the stack through
                       the output parameter item, but does not remove it
                       from the stack.
Description:           First, do error checking. Set the int being pointed
                       to by item to whatever value is going to be popped.
Input:                 this_Stack: a pointer to the stack from which to top
                       item: a pointer to a long storing the topped item.
Result:                Returns true upon success and false upon failure.
--------------------------------------------------------------------------*/
long top(Stack* this_Stack, long* item) {
    /* use to store stack pointer */   
    long stack_pointer;

    /* to check if the stack exists */
    if (this_Stack == NULL) {
        fprintf(stderr, TOP_NONEXIST);
        return 0;
    }

    /* to check the item exists */
    if (item == NULL) {
        fprintf(stderr, INCOMING_NONEXIST);
        return 0;
    }

    /* to check if the stack is empty */
    if (isempty_Stack(this_Stack) == 1) {
        fprintf(stderr, TOP_EMPTY);
        return 0;
    }

    /* store the stack pointer of the stack in long */
    stack_pointer = this_Stack[STACK_POINTER_INDEX]-1;
    /* to set the int pointed to by item */
    *item = this_Stack[stack_pointer];

    if (debug == 1) {
            if(*item>=0){
		    fprintf(stderr, TOP, (long)this_Stack[STACK_COUNT_INDEX], *item);
	    }
	    else{
		    fprintf(stderr, HEXTOP, (long)this_Stack[STACK_COUNT_INDEX], *item);
	    }
    }
    return 1;
}

FILE* write_Stack(Stack* this_Stack, FILE* stream) {

    long index = 0;         /* index into the stack */

    if (this_Stack == NULL) {
        fprintf(stderr, WRITE_NONEXIST_STACK);
        return stream;
    }

    if (stream == NULL) {
        fprintf(stderr, WRITE_NONEXIST_FILE);
        return stream;
    }

    if (stream == stderr)
        fprintf(stream, "Stack has %ld items in it.\n",
            get_occupancy(this_Stack));

    for (index = STACK_COUNT_INDEX + STACK_OFFSET;
        index < get_occupancy(this_Stack); index++) {

        if (stream == stderr)
            fprintf(stream, "Value on stack is |0x%lx|\n",
                this_Stack[index]);
        else {
            if (this_Stack[index] < 0)
                fprintf(stream, "%c ",
                (char)this_Stack[index]);
            else
                fprintf(stream, "%ld ", this_Stack[index]);
        }
    }

    return stream;
}
