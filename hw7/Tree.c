/***************************************************************************
% cs12wi20cu                         Homework 7              Tianling Luo
%--------------------------------------------------------------------------
% File: Tree.c
%
% Description: In the assignment, we will implement a binary tree data 
% structure. This structure will be used to allow variable assignment in 
% the calculator.
***************************************************************************/
#include <cstdlib>
#include <cstdio>  
#include <iostream>  
#include <string.h>  
#include "Tree.h"  
using namespace std;

#ifndef TRUE  
#define TRUE 1  
#endif  

#ifndef FALSE  
#define FALSE 0  
#endif  

static const char ALLOCATE[] = " - Allocating]\n";
static const char DEALLOCATE[] = " has been deallocated]\n";
static const char AND[] = " and ";
static const char COMPARE[] = " - Comparing ";
static const char INSERT[] = " - Inserting ";
static const char TREE[] = "[Tree ";

/***************************************************************************
% struct TNode
%
% Description: Nodes of the binary tree are represented by TNode objects.
% Each TNode contains the data stored in the TNode, height and balance
% information, and references to left and right children, as well as to
% the parent TNode. Each TNode of the Tree will have a hasBeenDeleted
% field.
%
% Data Fields:
%     data (Whatever)  - data to store in TNode
%     left (TNode)  - reference to left children
%     right (TNode)  - reference to right children
%     parent (TNode)  - reference to parent TNode
%     occupancy (long) - number of nodes
%     hasBeenDeleted (bool)  - track if deleted
%     balance (long) - balance of the node
%     height (long) - height of the node
%
% Public functions:
%     TNode - constructor
%     ~TNode - destructor
%     Write - Write a TNode to the String representation
%     Write_AllTNodes - Writes all TNodes to the String representation 
%          field.
%     delete_AllTNodes - delete all TNodes
***************************************************************************/

struct TNode {
    Base* data;
    TNode* left, * right, * parent;
    static long occupancy;
    unsigned long hasBeenDeleted;

    // left child's height - right child's height  
    long balance;

    // 1 + height of tallest child, or 0 for leaf  
    long height;

    TNode(Base* element) : data(element), left(0), right(0),
        parent(0), hasBeenDeleted(FALSE), balance(0), height(0) 
/***************************************************************************
% Routine Name : TNode
% File :         Tree.c
%
% Description :  Allocates and initializes the memory associated with the 
%		 TNode object. 
%
% Parameters descriptions :
%
% name               description
% ------------------ ------------------------------------------------------
% element            The data stored in TNode.
***************************************************************************/
	{
    		// increment on occupancy
    		occupancy++;
    }

    ~TNode(void) 
/***************************************************************************
% Routine Name : ~TNode
% File :         Tree.c
%
% Description :  Called when de-allocating this TNode.
***************************************************************************/

    {
	    // delete data
	    delete data;
	    // set pointers to 0
	    left=right=parent=0;
	    // decrement on occupancy
	    occupancy--;
     }

    ostream& Write(ostream& stream) const {
        stream << "at height:  " << height << "  with balance:  "
            << balance << "  ";
        return data->Write(stream) << "\n";
    }
    ostream& Write_AllTNodes(ostream& stream) const {
        if (left)
            left->Write_AllTNodes(stream);
        if (!hasBeenDeleted)
            Write(stream);
        if (right)
            right->Write_AllTNodes(stream);

        return stream;
    }

    void delete_AllTNodes(void) 
/***************************************************************************
% Routine Name : delete_AllTNodes
% File :         Tree.c
%
% Description :  Does a post-order traversal deleting all TNodes in the 
% 		 Tree. 
***************************************************************************/

    {
        // if left pointer is not 0
	if(left){
		// delete the left
		left->delete_AllTNodes();
	}
	if(right){
		// delete the right
		right->delete_AllTNodes();
	}
	delete this;
    }
};

// initialization of static data fields of Tree  
long TNode::occupancy = 0;
bool Tree::debug_on = 0;

void Tree::Set_Debug(bool option) 
/***************************************************************************
% Routine Name : Tree :: Set_Debug  (public)
% File :         Tree.c
%
% Description :  Set debug to be option for debugging messages
%
% Parameters descriptions :
%
% name               description
% ------------------ ------------------------------------------------------
% option             true or false according to debug
***************************************************************************/

{
    // set the debug_on to be option
    debug_on = option;
}

Tree::Tree(const char* name) : root(0), tree_name(strdup(name))
/***************************************************************************
% Routine Name : Tree :: Tree  (public)
% File :         Tree.c
%
% Description :  Initializes root pointer to NULL, and occupancy to 0.
%
% Parameters descriptions :
%
% name               description
% ------------------ ------------------------------------------------------
% name               Name of the tree.
***************************************************************************/

{
    // for debugging message
    if(debug_on){
	    cerr<<TREE<<tree_name<<ALLOCATE;   
    }	    
}

Tree :: ~Tree(void)
/***************************************************************************
% Routine Name : Tree :: ~Tree  (public)
% File :         Tree.c
%
% Description :  deallocates memory associated with the Tree.  It
%                will also delete all the memory of the elements within
%                the table.
***************************************************************************/

{
    // for debugging message
    if(debug_on){
	    cerr<<TREE<<tree_name<<DEALLOCATE;   
    }	

    if(root){
	    // call delete_AllTNodes
	    root->delete_AllTNodes();
    }
    free((char*)tree_name);
}   // end: ~Tree

unsigned long Tree::Insert(Base* element) 
/***************************************************************************
% Routine Name : Tree :: Insert  (public)
% File :         Tree.c
%
% Description : This function will insert element in the binary tree.
%       If the element cannot be inserted, false will be returned.
%       If the element can be inserted, the element is inserted
%       and true is returned.  Duplicate insertions will
%       cause the existing element to be deleted, and the duplicate
%       element to take its place.
%
% Parameters descriptions :
%
% name               description
% ------------------ ------------------------------------------------------
% element            The complete element to insert.
% <return>           true or false indicating success or failure of 
%			insertion
***************************************************************************/

{
	// use working to track in the tree
	TNode* working=root;
	// track left & right height
	int left_height;
	int right_height;
	// if inserting the first node
	if(!root){
		root=new TNode(element);
		// for debugging message
		if(debug_on){
			cerr<<TREE<<tree_name<<INSERT
	    			<<(const char*)*element<<"]"<<endl;
		}
		return true;
	}
	
	else while(true){
		// for debugging message
		if(debug_on){
			cerr<<TREE<<tree_name<<COMPARE<<(const char*)*element
				<<AND<<(const char*)*working->data<<"]"<<endl;
		}
		
		// if equal to current node
		if(*element==*(working->data)){
			// turn off the flag if it's on
			if(working->hasBeenDeleted==true){
    				working->hasBeenDeleted=false;
				TNode :: occupancy++;
			}
			// replace for duplicate
			working->data=element;
			break;
		}
		// if greater than current node
		else if (*element>*(working->data)){
			// if right child does not exist
			if(!working->right){
    				working->right=new TNode(element);
    				working->right->parent=working;
    				break;
			}
			// if right child exists
			else
    				working=working->right;
		}
		// if less than current node
		else{
			// if left child does not exist
			if(!working->left){
    				working->left=new TNode(element);
    				working->left->parent=working;
    				break;
			}
			// if left child exists
			else
    				working=working->left;
		}
	}
	
	// for debugging message
	if(debug_on){
		cerr<<TREE<<tree_name<<INSERT
    			<<(const char*)*element<<"]"<<endl;        
	}
	
	// loop of going up the tree
	while(working){
		// initialze for null
		left_height=-1;
		right_height=-1;
		working->height=0;
		working->balance=0;
		// if left child exists
		if(working->left)
			left_height=working->left->height;
		
		// if right child exists
		if(working->right)
			right_height=working->right->height;
		
		// if either left or right child exists
		if (working->left || working->right){
			working->height=(left_height>=right_height)?
				(left_height+1):(right_height+1);
			working->balance=left_height-right_height;
		}
		// go up the tree
		working=working->parent;
	}	
	return true;  
}

const Base* Tree::Lookup(const Base* element) const 
/***************************************************************************
% Routine Name : Tree :: Lookup  (public)
% File :         Tree.c
%
% Description : This function will lookup element in the binary tree.  If
%               found a pointer to the element is returned.  If the element
%               is not found, NULL will be returned to the user.
%
% Parameters descriptions :
%
% name               description
% ------------------ ------------------------------------------------------
% element            The complete element to lookup.
% <return>           A pointer to the element if found, else NULL
***************************************************************************/

{
    // use working to track in the tree
    TNode* working = root;

    // if root is null
    if (!root) {
        return NULL;
    }

    else while (true) {
        // for debugging message
        if (debug_on) {
            cerr<<TREE << tree_name << COMPARE
                << (const char*)*element << AND
                << (const char*)*working->data << "]"<<endl;
        }

        // if equal to current node
        if (*element==*working->data) {
            // check if the flag is on
            if (working->hasBeenDeleted)
                return NULL;
            else
                return working->data;
            }
        // if greater than current node
        else if (*element>*working->data) {
            // if right child does not exist
            if (!working->right) {
                return NULL;
            }
            // if right child exists
            else
                working = working->right;
        }
        // if less than current node
        else {
            // if left child does not exist
            if (!working->left) {
                return NULL;
            }
            // if left child exists
            else
                working = working->left;
        }
    }
    return NULL;    // not there
}

Base* Tree::Remove(const Base* element) 
/***************************************************************************
% Routine Name : Tree :: Remove  (public)
% File :         Tree.c
%
% Description : This function will remove element in the binary tree.  If
%               found a pointer to the element is returned.  If the element
%               is not found, NULL will be returned to the user.
%
% Parameters descriptions :
%
% name               description
% ------------------ ------------------------------------------------------
% element            The complete element to remove.
% <return>           A pointer to the element if found, else NULL
***************************************************************************/

{
    // use working to track in the tree
    TNode* working = root;

    // if root is null
    if (!root) {
        return NULL;
    }

    else while (true) {
        // for debugging message
        if (debug_on) {
            cerr << TREE << tree_name << COMPARE
                << (const char*)*element << AND
                << (const char*)*working->data << "]" << endl;
        }

        // if equal to current node
        if (*element==*working->data) {
            // turn on the flag if it's on
	    if(!working->hasBeenDeleted){
		    working->hasBeenDeleted = true;
		    TNode :: occupancy--;
		    return working->data;
	    }
	    else
		    break;
	}
        // if greater than current node
        else if (*element>*working->data) {
            // if right child does not exist
            if (!working->right) {
                return NULL;
            }
            // if right child exists
            else
                working = working->right;
        }
        // if less than current node
        else {
            // if left child does not exist
            if (!working->left) {
                return NULL;
            }
            // if left child exists
            else
                working = working->left;
        }
    }
    return NULL;    // not there
}

ostream& Tree::Write(ostream& stream) const
/***************************************************************************
% Routine Name : Tree :: Write (public)
% File :         Tree.c
%
% Description : This function will output the contents of the Tree table
%               to the stream specificed by the caller.  The stream could be
%               cerr, cout, or any other valid stream.
%
% Parameters descriptions :
%
% name               description
% ------------------ ------------------------------------------------------
% stream             A reference to the output stream.
% <return>           A reference to the output stream.
***************************************************************************/

{
    stream << "Tree " << tree_name << ":\n"
        << "occupancy is " << TNode::occupancy << " elements.\n";

    return (root) ? root->Write_AllTNodes(stream) : stream;
}       // end: Write
