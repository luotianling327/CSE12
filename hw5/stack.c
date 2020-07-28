/****************************************************************************

                                                        Tianling Luo
                                                        CSE12, winter
                                                        Feb 3, 2020
                                                        cs12wi20cu
                                Assignment Five

File Name:      stack.c
Description:    This program contains stack operations including,
delete_Stack, isempty_Stack, new_Stack, pop, push, top and write_Stack.
****************************************************************************/

#include <stdio.h>
#include "list.h"
#include "stack.h"

/*--------------------------------------------------------------------------
Function Name:         delete_Stack.
Purpose:               This destructor function deallocates all memory 
                       associated with the stack, including the memory 
                       associated with all of the nodes in the stack and sets
                       incoming pointer to NULL.
Description:           Call delete_List to deallocate and initialize the stack
                       to null.
Input:                 spp: a double pointer to the stack that is to be 
                       deleted.
Result:                No return value.
--------------------------------------------------------------------------*/
void delete_Stack(Stack** spp) {
    /* call delete_List to delete the Stack */
    delete_List(spp);
}

/*--------------------------------------------------------------------------
Function Name:         isempty_Stack.
Purpose:               Checks to see if this_stack is empty.
Description:           Call isempty_List to check if this_stack is empty.
Input:                 this_stack: a pointer to the stack we wish to check.
Result:                Return 1 if this_stack is empty and 0 otherwise.
--------------------------------------------------------------------------*/
long isempty_Stack(Stack* this_stack) {
    /* use isempty_List to check if this_Stack is empty */
    return isempty_List(this_stack);
}

/*--------------------------------------------------------------------------
Function Name:         new_Stack.
Purpose:               This constructor function allocates and initializes
                       a new Stack object.
Description:           Call new_List to allocate a new stack and initialize
                       it.
Input:                 copy_func: a pointer to the function which makes 
                       copies of the elements stored in this_stack.
                       delete_func: a pointer to the function which frees the
                       memory associated with elements stored in this_stack.
                       is_greater_than_func: a pointer to the function which
                       compares elements in this_stack.
                       write_func: a pointer to the function which writes
                       elements in this_stack.
Result:                Returns a pointer to the stack.
--------------------------------------------------------------------------*/
Stack* new_Stack(void* (*copy_func) (void*),
    void (*delete_func) (void*),
    long (*is_greater_than_func) (void*, void*),
    FILE* (*write_func) (void*, FILE*)) {

    /* call new_List the create a new Stack */
    return new_List(copy_func, delete_func,
        is_greater_than_func, write_func);
}

/*--------------------------------------------------------------------------
Function Name:         pop.
Purpose:               Removes an element in this_stack at the end.
Description:           Call remove_List to remove the last item in stack.
Input:                 this_stack: a pointer to the stack from which we wish
                       to remove.
Result:                Returns a pointer to an item at the end. Returns 0 
                       for failure.
--------------------------------------------------------------------------*/
void* pop(Stack* this_stack) {
    /* call remove_List and set the location to the END */
    return remove_List(this_stack, END);
}

/*--------------------------------------------------------------------------
Function Name:         push.
Purpose:               Push the element into this_stack at the end.
Description:           Call insert to insert to the end of the stack.
Input:                 this_stack: a pointer to the list we wish to check.
                       element: a pointer to the object we wish to push in
                       the stack.
Result:                Returns 1 or 0 for success or failure.
--------------------------------------------------------------------------*/
long push(Stack* this_stack, void* element) {
    /* call insert and set the location to the END */
    return insert(this_stack, element, END);
}

/*--------------------------------------------------------------------------
Function Name:         top.
Purpose:               Returns a pointer to the object stored at location
                       where for viewing.
Description:           Call view to see the last item in the stack.
Input:                 this_stack: a pointer to the stack we wish to check.
Result:                Returns a pointer to the object stored at the end for
                       viewing. Returns 0 for failure.
--------------------------------------------------------------------------*/
void* top(Stack* this_stack) {
    /* call view and set the location to the END */
    return view(this_stack, END);
}

/*--------------------------------------------------------------------------
Function Name:         write_Stack.
Purpose:               Writes the elements of this_list forwards, starting
                       with the first item.
Description:           Call write_List to write the elements.
Input:                 this_stack: a pointer to the stack we wish to write.
                       stream: either stdout or stderr.
Result:                Return the stream output.
--------------------------------------------------------------------------*/
FILE* write_Stack(Stack* this_stack, FILE* stream) {
    /* call write_Stack to write the elements in this_Stack */
    return write_List(this_stack, stream);
}
