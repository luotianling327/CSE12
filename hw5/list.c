/****************************************************************************

                                                        Tianling Luo
                                                        CSE12, winter
                                                        Feb 3, 2020
                                                        cs12wi20cu
                                Assignment Five

File Name:      list.c
Description:    In this assignment, I will create a polymorphic generic
container based on a linked-list data structure. In list.c, the List and
Node structs are defined. The List object contains data fields such as
occupancy and list_count, and also contains pointers to functions which
manipulate the objects stored in the list. The Node objects form a
doubly-linked list, where the pre pointer points to the previous item in
the list and the next pointer points to the next item in the list. The
list is circular.


Debugger Questions:

Using the debugger, insert three nodes into your list. 
List the values of the following pointers after the three insertions:

1. front: 0x6050f0
2. Node 1 data: 0x6050d0
3. Node 1 pre: 0x605070
4. Node 1 next: 0x6050b0
5. Node 2 data: 0x605090
6. Node 2 pre: 0x6050f0
7. Node 2 next: 0x605070
8. Node 3 data: 0x605050
9. Node 3 pre: 0x6050b0
10. Node 3 next: 0x6050f0
****************************************************************************/

#include <malloc.h>
#include <stdio.h>
#include "mylib.h"
#include "list.h"

typedef struct Node {
    struct Node* pre;      /* how to access the prior Node */
    struct Node* next;     /* how to access the next Node */
    void* data;            /* the data to store */
} Node;

typedef struct List {
    Node* front;             /* the front of the list */
    long list_count;        /* which list is it */
    long occupancy;
    void* (*copy_func) (void*);
    void (*delete_func) (void*);
    long (*is_greater_than_func) (void*, void*);
    FILE* (*write_func) (void*, FILE*);
} List;

/* private Node function declarations */
static void delete_Node(Node**, void (*delete_func) (void*));
static Node* insert_Node(Node*, void*, void* (*copy_func) (void*));
static int locate(List* this_list, void* element);
static Node* new_Node(void*, void* (*copy_func) (void*));
static void* remove_Node(Node*);
static void* view_Node(Node*);
static FILE* write_Node(Node*, FILE*, FILE* (*write_func) (void*, FILE*));

/* catastrophic error messages */
static const char ADNEXT_NONEXIST[] =
"Advance next from non-existent list!!!\n";
static const char ADNEXT_EMPTY[] =
"Advance next from empty list!!!\n";
static const char ADPRE_NONEXIST[] =
"Advance pre from non-existent list!!!\n";
static const char ADPRE_EMPTY[] =
"Advance pre from empty list!!!\n";
static const char DELETE_NONEXIST[] =
"Deleting from non-existent list!!!\n";
static const char DELETE_NONEXISTNODE[] =
"Deleting a non-existent node!!!\n";
static const char ISEMPTY_NONEXIST[] =
"Is empty check from non-existent list!!!\n";
static const char INSERT_NONEXIST[] =
"Inserting to a non-existent list!!!\n";
static const char REMOVE_NONEXIST[] =
"Removing from non-existent list!!!\n";
static const char REMOVE_EMPTY[] =
"Remove from empty list!!!\n";
static const char VIEW_NONEXIST[] =
"Viewing a non-existent list!!!\n";
static const char VIEW_NONEXISTNODE[] =
"Viewing a non-existent node!!!\n";
static const char VIEW_EMPTY[] =
"Viewing an empty list!!!\n";
static const char WRITE_NONEXISTFILE[] =
"Writing to a non-existent file!!!\n";
static const char WRITE_NONEXISTLIST[] =
"Writing from a non-existent list!!!\n";
static const char WRITE_MISSINGFUNC[] =
"Don't know how to write out elements!!!\n";
static const char WRITE_NONEXISTNODE[] =
"Writing from a non-existent node!!!\n";

/* debug messages */
static const char ADNEXT[] = "[List %ld - Advancing next]\n";
static const char ADPRE[] = "[List %ld - Advancing pre]\n";
static const char INSERT[] = "[List %ld - Inserting node]\n";
static const char REMOVE[] = "[List %ld - Removing node]\n";
static const char VIEW[] = "[List %ld - Viewing node]\n";
static const char LIST_ALLOCATE[] = "[List %ld has been allocated]\n";
static const char LIST_DEALLOCATE[] = "[List %ld has been deallocated]\n";

static int debug_on = FALSE;    /* allocation of debug flag */
static long list_counter = 0;   /* the number of lists allocated so far */

/*--------------------------------------------------------------------------
Function Name:         set_debug_on.
Purpose:               Turns on debugging for this list. 
Description:           Set the debug_on to be TRUE.
Input:                 No input.
Result:                No return value.
--------------------------------------------------------------------------*/
void set_debug_on(void) {
    /* turn on debugging */
    debug_on = TRUE;
    return;
}

/*--------------------------------------------------------------------------
Function Name:         set_debug_off.
Purpose:               Turns off debugging for this list. 
Description:           Set the debug_on to be FALSE.
Input:                 No input.
Result:                No return value.
--------------------------------------------------------------------------*/
void set_debug_off(void) {
    /* turn off debugging */
    debug_on = FALSE;
    return;
}

/*--------------------------------------------------------------------------
Function Name:         advance_next_List.
Purpose:               Advances the front pointer one node after where it
                       currently points.
Description:           Set the front pointer to the next node.
Input:                 this_list: a pointer to the list we wish to shift.
Result:                No return value.
--------------------------------------------------------------------------*/
void advance_next_List(List* this_list) {
    /* check if this_list is null */
    if (this_list == NULL) {
        fprintf(stderr, ADNEXT_NONEXIST);
        return;
    }

    /* to check if this_list is empty */
    if (isempty_List(this_list)) {
        fprintf(stderr, ADNEXT_EMPTY);
        return;
    }

    /* set the front pointer to the next node */
    this_list->front = this_list->front->next;

    /* for debug messages */
    if (debug_on) {
        fprintf(stderr, ADNEXT, this_list->list_count);
    }

    return;
}

/*--------------------------------------------------------------------------
Function Name:         Advance_pre_List.
Purpose:               Advances the front pointer one node before where it
                       currently points.
Description:           Set the front pointer to the pre node.
Input:                 this_list: a pointer to the list we wish to shift.
Result:                No return value.
--------------------------------------------------------------------------*/
void advance_pre_List(List* this_list) {
    /* check if this_list is null */
    if (this_list == NULL) {
        fprintf(stderr, ADPRE_NONEXIST);
        return;
    }

    /* to check if this_list is empty */
    if (isempty_List(this_list)) {
        fprintf(stderr, ADPRE_EMPTY);
        return;
    }

    /* set the front pointer to the pre node */
    this_list->front = this_list->front->pre;

    /* for debug messages */
    if (debug_on) {
        fprintf(stderr, ADPRE, this_list->list_count);
    }

    return;
}

/*--------------------------------------------------------------------------
Function Name:         delete_List.
Purpose:               This destructor function deallocates all memory 
                       associated with the list, including the memory 
                       associated with all of the nodes in the list and sets
                       incoming pointer to NULL.
Description:           Use a loop of delete_Node, delete Node and the data.
Input:                 lpp: a double pointer to the list that is to be deleted.
Result:                No return value.
--------------------------------------------------------------------------*/
void delete_List(List** lpp) {
    /* use this_list to denote the list to be deleted */
    List* this_list;
    /* use working to track the current node */
    Node* working;
	
    /* check if the list exist */
    if (!lpp || !*lpp) {
        fprintf(stderr, DELETE_NONEXIST);
        return;
    }

    this_list = *lpp;

    /* for debug messages */
    if (debug_on) {
        fprintf(stderr, LIST_DEALLOCATE, this_list->list_count);
    }

    while (this_list->occupancy != 0) {
	/* to track the current node */
        working = this_list->front;
	this_list->front=this_list->front->pre;
        delete_Node(&working, this_list->delete_func);
        this_list->occupancy--;
    }

    /* free the list */
    free(*lpp);

    /* assign pointer to null */
    *lpp = NULL;

    /* decrement on list_counter */
    list_counter--;
    return;
}

/*--------------------------------------------------------------------------
Function Name:         insert.
Purpose:               Inserts the element into this_list either at the
                       front, end or in sorted form.
Description:           locate should be used to locate where the node should
                       go in the list. This function should then call
                       insert_Node to incorporate the Node into the list.
Input:                 this_list: a pointer to the list we wish to check.
                       element: a pointer to the object we wish to insert in
                       the list.
                       where: the place in the list where element should be 
                       stored after the call to insert (1 for first item,
                       0 for last item, 2 for sorted insertion of the item).
Result:                Returns 1 or 0 for success or failure.
--------------------------------------------------------------------------*/
long insert(List* this_list, void* element, long where) {
    /* use location to store the return value from locate */
    int location;
	
    /* check if this_list is null */
    if (this_list == NULL) {
        fprintf(stderr, INSERT_NONEXIST);
        return 0;
    }

    /* for the front case */
    if (where == FRONT || this_list->occupancy == 0) {
        /* call insert_Node to insert at the front */
        this_list->front = insert_Node(this_list->front, element,
            this_list->copy_func);
    }

    /* for the end case */
    else if (where == END) {
        /* call insert_Node to insert at the end */
        insert_Node(this_list->front, element, this_list->copy_func);
    }

    /* for the sorted case */
    else if (where == SORTED) {
        /* save prior value of front to be able to
         * restore after the insertion in complete */
        Node* temp = this_list->front;
        /* call locate function */
        location=locate(this_list, element);
	if(location==1){
        	/* call insert_Node */
        	insert_Node(this_list->front, element, 
				this_list->copy_func);
		/* restore the front pointer */
        	this_list->front = temp;
	}
	else{
		/* call insert_Node */
		this_list->front = insert_Node
			(this_list->front, element, 
			 this_list->copy_func);
            }
    }
    this_list->occupancy++;

    /* for debug messages */
    if (debug_on) {
        fprintf(stderr, INSERT, this_list->list_count);
    }

    return 1;
}

/*--------------------------------------------------------------------------
Function Name:         locate.
Purpose:               locate should be used to locate where the node should
                       be in a sorted list.
Description:           Use a loop and call advance_next_List until reached 
                       the desired location.
Input:                 this_list: a pointer to the list we wish to check.
                       element: a pointer to the object we wish to insert in
                       the list.
Result:                Result is expected to be true inserting at the front
                       of the list.
--------------------------------------------------------------------------*/
static int locate(List* this_list, void* element) {
    /* use iteration for for loop */
    int iteration;
    if(!this_list->is_greater_than_func(element,this_list->front->data))
	    return 0;
    /* use a loop */
    for (iteration = 0; iteration < this_list->occupancy; iteration++) {
        /* call advance_next_List in the loop until reach the desired
         * location */
        if (this_list->is_greater_than_func(element, this_list->front->data))
            advance_next_List(this_list);
        else
            break;
    }
    return 1;
}

/*--------------------------------------------------------------------------
Function Name:         isempty_List.
Purpose:               Checks to see if this_list is empty.
Description:           Check if the occupancy is 0.
Input:                 this_list: a pointer to the list we wish to check.
Result:                Return 1 if this_list is empty and 0 otherwise.
--------------------------------------------------------------------------*/
long isempty_List(List* this_list) {
    /* check if this_list is null */
    if (this_list == NULL) {
        fprintf(stderr, ISEMPTY_NONEXIST);
        return 0;
    }

    /* check if the occupancy is 0 */
    if (this_list->occupancy == 0) {
        return 1;
    }
    else
        return 0;
}

/*--------------------------------------------------------------------------
Function Name:         new_List.
Purpose:               This constructor function allocates and initializes
                       a new List object.
Description:           Initialize this_List to be an empty list.
Input:                 copy_func: a pointer to the function which makes 
                       copies of the elements stored in this_list.
                       delete_func: a pointer to the function which frees the
                       memory associated with elements stored in this_list.
                       is_greater_than_func: a pointer to the function which
                       compares elements in this_list.
                       write_func: a pointer to the function which writes
                       elements in this_list.
Result:                Returns a pointer to the list.
--------------------------------------------------------------------------*/
List* new_List(
    void* (*copy_func) (void*),
    void (*delete_func) (void*),
    long (*is_greater_than_func) (void*, void*),
    FILE* (*write_func) (void*, FILE*)) {

    /* allocate */
    List* this_list = (List*)malloc(sizeof(List));
    /* initialize */
    this_list->front = NULL;
    list_counter++;
    this_list->list_count = list_counter;
    this_list->occupancy = 0;
    this_list->copy_func = copy_func;
    this_list->delete_func = delete_func;
    this_list->is_greater_than_func = is_greater_than_func;
    this_list->write_func = write_func;

    /* for debug messages */
    if (debug_on) {
        fprintf(stderr, LIST_ALLOCATE, this_list->list_count);
    }

    return this_list;
}

/*--------------------------------------------------------------------------
Function Name:         remove_List.
Purpose:               Removes an element in this_list at location where.
Description:           This function should call remove_Node to restructure
                       the list to remove the node. 
Input:                 this_list: a pointer to the list from which we wish
                       to remove.
                       where: the place in the number of the element in the
                       list we wish to remove insert (1 for first item, 
                       0 for last item).
Result:                Returns a pointer to an item at the specified
                       location. Returns 0 for failure.
--------------------------------------------------------------------------*/
void* remove_List(List* this_list, long where) {
    /* use rem to store returned value of remove_Node */
    void* rem;

    /* check if this_list is null */
    if (this_list == NULL) {
        fprintf(stderr, REMOVE_NONEXIST);
        return 0;
    }

    /* check if this_list is empty */
    if (isempty_List(this_list)) {
        fprintf(stderr, REMOVE_EMPTY);
        return 0;
    }

    /* for debug messages */
    if (debug_on) {
        fprintf(stderr, REMOVE, this_list->list_count);
    }

    /* if remove insert the first item */
    if (where == FRONT) {
	this_list->front=this_list->front->next;
	rem=remove_Node(this_list->front->pre);
	this_list->occupancy--;
	if(this_list->occupancy==0)
		this_list->front=NULL;
        return rem;
    }

    /* if remove insert the last item */
    if (where == END) {
	rem=remove_Node(this_list->front->pre);
	this_list->occupancy--;
	if(this_list->occupancy==0)
		this_list->front=NULL;
	return rem;
    }

    else
        return 0;
}

/*--------------------------------------------------------------------------
Function Name:         view.
Purpose:               Returns a pointer to the object stored at location
                       where for viewing.
Description:           This function should call view_Node, which returns the
                       data.
Input:                 this_list: a pointer to the list we wish to check.
                       where: the place in the list which holds the element
                       to view (1 for first item, 0 for last item).
Result:                Returns a pointer to the object stored at location
                       where for viewing. Returns 0 for failure.
--------------------------------------------------------------------------*/
void* view(List* this_list, long where) {
    /* check if this_list is null */
    if (this_list == NULL) {
        fprintf(stderr, VIEW_NONEXIST);
        return 0;
    }

    /* check if this_list is null */
    if (isempty_List(this_list)) {
        fprintf(stderr, VIEW_EMPTY);
        return 0;
    }

    /* for debug messages */
    if (debug_on) {
        fprintf(stderr, VIEW, this_list->list_count);
    }

    /* if viewing the first item */
    if (where == FRONT) {
        /* call view_Node to return the data */
        view_Node(this_list->front);
        return view_Node(this_list->front);
    }

    /* if viewing the last item */
    if (where == END) {
        /* call view_Node to return the data */
        view_Node(this_list->front->pre);
        return view_Node(this_list->front->pre);
    }

    else
        return 0;
}

FILE* write_List(List* this_list, FILE* stream) {
    long count;             /* to know how many elements to print */
    Node* working;                 /* working node */

    if (!stream) {
        fprintf(stderr, WRITE_NONEXISTFILE);
        return NULL;
    }

    if (!this_list) {
        fprintf(stderr, WRITE_NONEXISTLIST);
        return NULL;
    }

    if (stream == stderr)
        fprintf(stream, "List %ld has %ld items in it.\n",
            this_list->list_count, this_list->occupancy);

    if (!this_list->write_func) {
        fprintf(stream, WRITE_MISSINGFUNC);
        return stream;
    }

    if (this_list->occupancy >= 1)
        working = this_list->front;

    for (count = 1; count <= this_list->occupancy; count++) {
        if (stream == stderr)
            fprintf(stream, "\nelement %ld:  ", count);
        write_Node(working, stream, this_list->write_func);
        working = working->next;
    }

    return stream;
}

/*--------------------------------------------------------------------------
Function Name:         write_reverse_List.
Purpose:               Writes the elements of this_list backwards, starting
                       with the last item.
Description:           Go from the back of the list by starting from
                       this_list->front->pre and then go to the first one by 
                       pre.
Input:                 this_list: a pointer to the list we wish to write.
                       stream: either stdout or stderr.
Result:                Return the stream output.
--------------------------------------------------------------------------*/
FILE* write_reverse_List(List* this_list, FILE* stream) {
    /* to know how many elements to print */
    long count;
    /* working node */
    Node* working;

    /* check if the stream exists */
    if (!stream) {
        fprintf(stderr, WRITE_NONEXISTFILE);
        return NULL;
    }

    /* check if this_list exists */
    if (!this_list) {
        fprintf(stderr, WRITE_NONEXISTLIST);
        return NULL;
    }

    /* for debugging message */
    if (stream == stderr)
        fprintf(stream, "List %ld has %ld items in it.\n",
            this_list->list_count, this_list->occupancy);

    /* if the write_function is missing */
    if (!this_list->write_func) {
        fprintf(stream, WRITE_MISSINGFUNC);
        return stream;
    }

    if (this_list->occupancy >= 1)
        working = this_list->front->pre;

    for (count = 1; count <= this_list->occupancy; count++) {
        if (stream == stderr)
            fprintf(stream, "\nelement %ld:  ", count);
	/* call write_Node to write */
        write_Node(working, stream, this_list->write_func);
	/* go to the previous node */
        working = working->pre;
    }

    return stream;
}

static void delete_Node(Node** npp, void (*delete_func) (void*)) {

    /* does the node exist??? */
    if (!npp || !*npp) {
        fprintf(stderr, DELETE_NONEXISTNODE);
        return;
    }

    /* call function to delete element */
    if (delete_func && (*npp)->data)
        (*delete_func) (&((*npp)->data));

    /* delete element */
    free(*npp);

    /* assign node to NULL */
    *npp = NULL;
}

/*--------------------------------------------------------------------------
Function Name:         insert_Node.
Purpose:               Creates a new node to hold element, or, if copy_func
                       is non-NULL, a copy of element.
Description:           First, cbeck if this_Node is NULL. Then, attach the
                       new node and integrate it.
Input:                 this_Node: a pointer to the Node that the new node
                       is inserted BEFORE.
                       element: a pointer to the object we wish to store in
                       the new node.
                       copy_func: the pointer to copy function passed into the
                       list constructor or NULL if no copy needs to be made.
Result:                Return the node with element inserted.
--------------------------------------------------------------------------*/
static Node* insert_Node(Node* this_Node, void* element,
    void* (*copy_func) (void*)) {
    /* create a new node to hold elements */
    Node* working = new_Node(element, copy_func);
    /* case of inserting the first item */
    if (this_Node == NULL) {
        working->pre = working;
        working->next = working;
    }
    /* case of inserting every other item */
    else{
            /* attach the new node */
            working->pre = this_Node->pre;
            working->next = this_Node;
            /* integrate */
            this_Node->pre->next = working;
            this_Node->pre = working;
        }
    return working;
}

static Node* new_Node(void* element, void* (*copy_func) (void*)) {

    /* allocate memory */
    Node* this_Node = (Node*)malloc(sizeof(Node));

    /* initialize memory */
    this_Node->next = this_Node->pre = NULL;
    this_Node->data = (copy_func) ? (*copy_func) (element) : element;

    return this_Node;
}

/*--------------------------------------------------------------------------
Function Name:         remove_Node.
Purpose:               "Unlinks" this_Node from the list.
Description:           Arrange the pointers of the surrounding Nodes so
                       they no longer point to this_Node.
Input:                 this_Node: a pointer to the Node we wish to remove
                       from the list.
Result:                A pointer to the data is returned.
--------------------------------------------------------------------------*/
static void* remove_Node(Node* this_Node) {
    /* store the pointer to data */
    Node* p = this_Node->data;
    /* arrange the pointers of pre nodes */
    this_Node->pre->next = this_Node->next;
    /* arrange the pointers of next nodes */
    this_Node->next->pre = this_Node->pre;
    /* free memory associated with Node */
    delete_Node(&this_Node,0);

    return p;
}

/*--------------------------------------------------------------------------
Function Name:         view_Node.
Purpose:               Returns a pointer to this_Node's data.
Description:           Returns a pointer to this_Node's data.
Input:                 this_Node: a pointer to the Node whose data we wish
                       to view.
Result:                Returns a pointer to this_Node's data.
--------------------------------------------------------------------------*/
static void* view_Node(Node* this_Node) {
    /* check if this_Node is null */
    if (this_Node == NULL) {
        fprintf(stderr, VIEW_NONEXISTNODE);
    }

    /* return a pointer to this_Node's data */
    return this_Node->data;
}

static FILE* write_Node(Node* this_Node, FILE* stream,
    FILE* (*write_func) (void*, FILE*)) {

   if (!stream) {
       fprintf(stderr, WRITE_NONEXISTFILE);
       return NULL;
   }

   if (!this_Node) {
       fprintf(stream, WRITE_NONEXISTNODE);
       return stream;
   }

   if (!write_func) {
       fprintf(stream, WRITE_MISSINGFUNC);
       return stream;
   }

   return (*write_func) (this_Node->data, stream);
}
