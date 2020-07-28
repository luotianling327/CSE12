#ifndef LIST_H
#define LIST_H

/* DO NOT ADD CODE TO THIS FILE: This file is used in evaluation
 * Changing function signatures will result in a 25% deduction.
 * adding comments is ok.  Changing it in other ways will cause
 * you deductions or a score of 0 on your assignment.
 * YOU HAVE BEEN WARNED!!!
 */

#include <stdio.h>

struct List;

void advance_next_List (struct List *); /* advances the front pointer one node
                                after where it currently points */
void advance_pre_List (struct List *); /* advances the front pointer one node
                                before where it currently points */
void delete_List (struct List **); /* deallocates list and all nodes and
                                elements and sets incoming pointer to NULL */
long insert (struct List *, void * element, long where); /* inserts element 
                                into list at specified location.
                                Returns 1 or 0 for success or failure */
long isempty_List (struct List *); /* returns 1 if list is empty, 0
                                otherwise */

struct List * new_List (void *(*copy_func) (void *), /* allocate, initialize */
        void (*delete_func) (void *),
        long (*is_greater_than_func) (void *, void *),
        FILE *(*write_func) (void *, FILE *));

void * remove_List (struct List *, long where); /* removes and returns a
                                pointer to an item at the specified location.
                                Returns 0 for failure */
void * view (struct List *, long where); /* returns a pointer to an item at the
                                specified location without effecting list.
                                Returns 0 for failure */
FILE * write_List (struct List *, FILE *); /* displays list to FILE */
FILE * write_reverse_List (struct List *, FILE *);/* displays list in reverse */

void set_debug_on (void);       /* turns list debugging on */ 
void set_debug_off (void);      /* turns list debugging off */ 

/* where to insert, view, remove */
#define FRONT 1
#define END 0 
/* where to insert */
#define SORTED 2 

#endif
