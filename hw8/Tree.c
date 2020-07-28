//==========================================================================
// cs12wi20cu                         Homework 8              Tianling Luo
//--------------------------------------------------------------------------
// File: Tree.c
//
// Description: In this assignment, we will implement a binary tree data
// structure to use the "template" feature of C++. 
//==========================================================================

#include <cstdlib>
#include <string>
#include "Tree.h"
using namespace std;

// messages
static const char AND[] = " and ";
static const char CHECK[] = " - Checking ";
static const char COMPARE[] = " - Comparing ";
static const char DEALLOCATE[] = " - Deallocating]\n";
static const char INSERT[] = " - Inserting ";
static const char REPLACE[] = " - Replacing ";
static const char UPDATE[] = " - Updating ";
// set static counter
static int counter = 0;

template <class Whatever>
int Tree<Whatever>::debug = 0;

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#define THRESHOLD 2

//=========================================================================
// class Tree
//
// Description: The Tree class is defined and it contains the member
// functions that will be called by the Tree.c code.
//
// Data Fields:
//     TNode (friend struct)  - The tree node
//     occupancy (long)  - the occupancy in the tree
//     root (TNode*) - the root of the tree
//     tree_count (unsigned long) - tree count
//     debug (static int) - option to set debug message
//
// Public functions:
//     Tree - constructor
//     ~Tree - destructor
//     static void Set_Debug_On - set debug on
//     static void Set_Debug_Off - set debug off
//     unsigned long Insert - Insert nodes into the tree
//     unsigned long Lookup - Look up nodes in the tree
//     unsigned long Remove - Remove nodes from the tree
//     Write - Write in in-order traversal
//========================================================================== 

template <class Whatever>
ostream& operator << (ostream&, const TNode<Whatever>&);

//=========================================================================
// struct TNode
//
// Description: The TNode structure is defined and it contains the member
// functions that will be called by the Tree.c code.
//
// Data Fields:
//     balance (long)  - balance of the node
//     data (Whatever)  - data to store in TNode
//     height (long) - height of the node
//     left (TNode*) - pointer to the left
//     occupancy (long) - number of nodes
//     right (TNode*) - pointer to the right
//     tree_count (unsigned long) - count of the tree
//
// Public functions:
//     TNode - constructor for the root TNode
//     TNode - constructor for all other TNodes
//     ~TNode - destructor
//     void delete_AllTNodes - delete all the nodes
//     unsigned long Insert - insert nodes
//     unsigned long Lookup - lookup nodes
//     void ReplaceAndRemoveMax - replace TNode with max TNode in left
//           subtree
//     unsigned long Remove - remove nodes
//     void SetHeightAndBalance - Update height and balance
//     Write_AllTNodes - Write in in-order traversal
//==========================================================================  
template <class Whatever>
struct TNode {
    long balance;
    Whatever data;
    long height;
    TNode<Whatever>* left;
    long& occupancy;
    TNode<Whatever>* right;
    unsigned long& tree_count;

//==========================================================================
// Routine Name : TNode
// File :         Tree.c
//
// Description :  Called when creating the root TNode. 
//
// Parameters descriptions :
//
// name               description
// ------------------ ------------------------------------------------------
// element            the data to be stored in the TNode
// theTree            a reference to the empty Tree where the root TNode
//                    is about to be created
//==========================================================================
    TNode(const Whatever& element, Tree<Whatever>& theTree)
        : balance(0), data(element), height(0), left(0),
        occupancy(theTree.occupancy), right(0),
        tree_count(theTree.tree_count) {
        // increment on occupancy
        occupancy++;
    }

//==========================================================================
// Routine Name : TNode
// File :         Tree.c
//
// Description :  Called when creating any other TNodes. 
//
// Parameters descriptions :
//
// name               description
// ------------------ ------------------------------------------------------
// element            the data to be stored in the TNode
// parentTNode        a reference to the parent of the TNode to be created
//==========================================================================
    TNode(const Whatever& element, TNode<Whatever>& parentTNode)
        : balance(0), data(element), height(0), left(0),
        occupancy(parentTNode.occupancy), right(0),
        tree_count(parentTNode.tree_count) {
        // increment on occupancy
        occupancy++;
    }

//==========================================================================
// Routine Name : ~TNode
// File :         Tree.c
//
// Description :  Called when de-allocating a TNode.
//==========================================================================
    ~TNode(void) {
        // decrement on occupancy
        occupancy--;
    }

//==========================================================================
// Routine Name : delete_AllTNodes
// File :         Tree.c
//
// Description : Performs a post-order traversal through the Tree deleting
//               each TNode. 
//==========================================================================
    void delete_AllTNodes(void) {
        // if left pointer exists
        if (left) {
            // delete the left
            left->delete_AllTNodes();
        }
	// if right pointer exists
        if (right) {
            // delete the right
            right->delete_AllTNodes();
        }
        delete this;
    }

    unsigned long Insert(const Whatever& element,
        TNode<Whatever>*& PointerInParent);

//==========================================================================
// Routine Name : Lookup
// File :         Tree.c
//
// Description : Lookup the matching data from the binary tree.
//
// Parameters descriptions :
//
// name               description
// ------------------ ------------------------------------------------------
// element            the data to be stored in the TNode
// <return>           Returns true or false indicating success of lookup
//==========================================================================
    unsigned long Lookup(Whatever & element) const
    {
        // store the return value
        long status = 0;
        // for debug message
        if (Tree<Whatever>::debug) {
            cerr << TREE << tree_count << COMPARE <<
                (const char*)(element) << AND << (const char*)data
                << "]" << endl;
        }
        if (element == data) {
            // update the data
	    element=data;
            status = 1;
        }

        // check to go right
        else if (element > data) {
            // if right exists
            if (right){
                // recursion on the right
                status = right->Lookup(element);
            }
        }

        // go left
        else {
            // if left exists
            if (left) {
                // recursion on the left
                status = left->Lookup(element);
            }
        }
        return status;
    }

//==========================================================================
// Routine Name : ReplaceAndRemoveMax
// File :         Tree.c
//
// Description : Called when removing a TNode with 2 children, replaces
// that TNode with the maximum TNode in its left subtree to maintain the
// Tree structure. 
//
// Parameters descriptions :
//
// name               description
// ------------------ ------------------------------------------------------
// targetTNode        a reference to the TNode to remove that has 2 children
// PointerInParent    a reference to the TNode pointer in the parent TNode 
//                    used to get to the current TNode
//==========================================================================
    void ReplaceAndRemoveMax(TNode<Whatever>& targetTNode,
        TNode<Whatever>*& PointerInParent) {
        // for debug message
        if (Tree<Whatever>::debug) {
            cerr << TREE << tree_count << CHECK <<
                (const char*)(data) << "]" << endl;
            cerr << TREE << tree_count << REPLACE <<
                (const char*)(data) << "]" << endl;
        }
        // if right is not null
        if (right) {
            // call to right recursively
            right->ReplaceAndRemoveMax(targetTNode, right);
	    // set height and balance
            SetHeightAndBalance(PointerInParent);
        }
        // if right is null
        else {
            PointerInParent = left;
            // reassign the data to targetTNode
            targetTNode.data = data;
            // delete
            delete this;
        }
    }

//==========================================================================
// Routine Name : Remove
// File :         Tree.c
//
// Description : Removes the matching data from the binary tree.
//
// Parameters descriptions :
//
// name               description
// ------------------ ------------------------------------------------------
// elementTNode       a reference to the TNode containing the data that
//                    identifies the element to remove
// PointerInParent    a reference to the TNode pointer in the parent TNode
//                    used to get to the current TNode
// fromSHB            true or false, keeping track of whether or not Remove
//                    was called from SetHeightAndBalance so that Remove
//                    can determine whether to call SetHeightAndBalance
// <return>           Returns true or false indicating success of removal
//==========================================================================
    unsigned long Remove(TNode<Whatever>& elementTNode,
        TNode<Whatever>*& PointerInParent,
        long fromSHB = FALSE) {
	// track the return value
	long status=0;
        // for debug message
        if (Tree<Whatever>::debug) {
            cerr << TREE << tree_count << COMPARE <<
                (const char*)(elementTNode.data) << AND 
                << (const char*)data << "]" << endl;
        }
        // calls == on data to compare names
        if (elementTNode.data == data) {
	    // update the data
	    elementTNode.data=data;
	    status=1;
            // if only left child exists
            if (left && !right) {
                // PIP is left
                PointerInParent = left;
                delete this;
            }

            // if only right child exists
            else if (right && !left) {
                // PIP is right
                PointerInParent = right;
                delete this;
            }

            // if with no child
            else if (!right && !left) {
                // PIP is null
                PointerInParent = NULL;
                delete this;
            }

            // if with two children
            else {
                // call ReplaceAndRemoveMax
                left->ReplaceAndRemoveMax(*this, left);
            }
        }

        // go right
        else if (elementTNode.data > data) {
            // if right is not null
            if (right){
                // call recursively
                status=right->Remove(elementTNode, right);
		// determine to call SetHeightAndBalance or not
       	        if (status==1 && !fromSHB) {
            		SetHeightAndBalance(PointerInParent);
		}
            }
        }

        //go left
        else {
            // if left is not null
            if (left){
                // call recursively
                status=left->Remove(elementTNode, left);
		// determine to call SetHeightAndBalance or not
        	if (status==1 && !fromSHB) {
            		SetHeightAndBalance(PointerInParent);
		}
            }
        }

        return status;
    }

//==========================================================================
// Routine Name : SetHeightAndBalance
// File :         Tree.c
//
// Description : Updates the height and balance of the current TNode.
//
// Parameters descriptions :
//
// name               description
// ------------------ ------------------------------------------------------
// PointerInParent    a reference to the TNode pointer in the parent TNode
//                    used to get to the current TNode.
//==========================================================================
    void SetHeightAndBalance(TNode<Whatever>*& PointerInParent) {
        // intialize left and right height
        long leftHeight = -1;
        long rightHeight = -1;
        // for debug message
        if (Tree<Whatever>::debug) {
            cerr << TREE << tree_count << UPDATE <<
                (const char*)(data) << "]" << endl;
        }

        // if left exists
        if (left) {
            leftHeight = left->height;
        }

        // if right exists
        if (right) {
            rightHeight = right->height;
        }

        // set height
        height = (leftHeight >= rightHeight) ?
            (leftHeight + 1) : (rightHeight + 1);

        // set balance
        balance = leftHeight - rightHeight;

        // if is unbalanced
        if (abs(balance) > THRESHOLD) {
            TNode<Whatever> elementTNode(data, *this);
            // remove and reinsert
            Remove(elementTNode, PointerInParent, true);
            PointerInParent->Insert(elementTNode.data, PointerInParent);
        }

    }

    ostream& Write_AllTNodes(ostream& stream) const {
        if (left)
            left->Write_AllTNodes(stream);
        stream << *this;
        if (right)
            right->Write_AllTNodes(stream);

        return stream;
    }
};

//==========================================================================
// Routine Name : Tree :: Set_Debug_On  (public)
// File :         Tree.c
//
// Description :  Set debug on
//==========================================================================
template <class Whatever>
void Tree<Whatever>::Set_Debug_On()
{
    // set the debug on
    debug = 1;
}

//==========================================================================
// Routine Name : Tree :: Set_Debug_Off  (public)
// File :         Tree.c
//
// Description :  Set debug off
//==========================================================================
template <class Whatever>
void Tree<Whatever>::Set_Debug_Off()
{
    // set the debug off
    debug = 0;
}

//==========================================================================
// Routine Name : TNode<Whatever> ::Insert
// File :         Tree.c
//
// Description : Inserts an element into the binary tree.
//
// Parameters descriptions :
//
// name               description
// ------------------ ------------------------------------------------------
// element            the data to be stored in the TNode
// PointerInParent    a reference to the TNode pointer in the parent TNode
//                    used to get to the current TNode.
// <return>           true or false indicating success of insertion
//==========================================================================
template <class Whatever>
unsigned long TNode<Whatever> ::Insert(const Whatever& element,
    TNode<Whatever>*& PointerInParent) {
    // for debug message
    if (Tree<Whatever>::debug) {
        cerr << TREE << tree_count << COMPARE <<
            (const char*)(element) << AND << (const char*)data 
            << "]" << endl;
    }
    // duplicate case
    if (element == data) {
        data = element;
        return 1;
    }

    //check to go right
    else if (element > data) {
        // if right child doesn't exist
        if (!right) {
            // for debug message
            if (Tree<Whatever>::debug) {
                cerr << TREE << tree_count << INSERT <<
                    (const char*)(element) << "]" << endl;
            }
            right = new TNode<Whatever>(element, *this);
        }
        else
            // recursion on the right
            right->Insert(element,right);
    }

    // go left
    else {
        // if left child doesn't exist
        if (!left) {
            // for debug message
            if (Tree<Whatever>::debug) {
                cerr << TREE << tree_count << INSERT <<
                    (const char*)(element) << "]" << endl;
            }
            left = new TNode<Whatever>(element, *this);
        }
        else
            // recursion on the left
            left->Insert(element,left);
    }

    // set height and balance
    SetHeightAndBalance(PointerInParent);

    return 1;
}

template <class Whatever>
ostream& operator << (ostream& stream, const TNode<Whatever>& nnn) {
    stream << "at height:  :" << nnn.height << " with balance:  "
        << nnn.balance << "  ";
    return stream << nnn.data << "\n";
}

//==========================================================================
// Routine Name : Tree<Whatever> ::IsEmpty
// File :         Tree.c
//
// Description :  Checking if the tree is empty or not. 
//
// Parameters descriptions :
//
// name               description
// ------------------ ------------------------------------------------------
// <return>           true if is empty false otherwise.
//==========================================================================
template <class Whatever>
unsigned long Tree<Whatever> ::IsEmpty() const {
    // if is empty
    if (!root) {
        return 1;
    }
    return 0;
}

//==========================================================================
// Routine Name : Tree<Whatever> ::Insert
// File :         Tree.c
//
// Description : Inserts an element into the binary tree. Inserts at the
// root TNode if Tree is empty, otherwise delegates to TNode's Insert.
//
// Parameters descriptions :
//
// name               description
// ------------------ ------------------------------------------------------
// element            the data stored in the TNode.
// <return>           true or false indicating success of insertion.
//==========================================================================
template <class Whatever>
unsigned long Tree<Whatever> ::Insert(const Whatever& element) {
    // if root is null
    if (!root) {
        // for debug message
        if (Tree<Whatever>::debug) {
            cerr << TREE << tree_count << INSERT << 
                (const char*)(element) << "]" << endl;
        }
        root = new TNode<Whatever>(element, *this);
        return 1;
    }
    // if root exists
    else {
        return(root->Insert(element, root));
    }
}

//==========================================================================
// Routine Name : Tree<Whatever> ::Lookup
// File :         Tree.c
//
// Description : Searches for an element in the Tree. Delegates to TNode's
//               Lookup when implemented recursively.
//
// Parameters descriptions :
//
// name               description
// ------------------ ------------------------------------------------------
// element            the data stored in the TNode
// <return>           Returns true or false indicating success of look up
//==========================================================================
template <class Whatever>
unsigned long Tree<Whatever> ::Lookup(Whatever& element) const {
    // to store the return value
    long status = 0;	
    // if root is null
    if (!root)
        return 0;
    // root is not null
    else {
        // call TNode's Lookup
        status=root->Lookup(element);
    }
    return status;
}

//==========================================================================
// Routine Name : Tree<Whatever> ::Remove
// File :         Tree.c
//
// Description : Removes an element from the Tree. Delegates to TNode's
//               Remove when Tree is not empty.
//
// Parameters descriptions :
//
// name               description
// ------------------ ------------------------------------------------------
// element            the data stored in the TNode
// <return>           Returns true or false indicating success of removal
//==========================================================================
template <class Whatever>
unsigned long Tree<Whatever> ::Remove(Whatever& element) {
    // to store the return value
    long status = 0;
    // if root is null
    if (!root)
        return 0;
    else {
        TNode<Whatever> elementTNode(element, *this);
        // delegate to TNode's Remove
        status = root->Remove(elementTNode, root);
	// update element
	element=elementTNode.data;
    }
    return status;
}

//==========================================================================
// Routine Name : Tree<Whatever> :: Tree  (public)
// File :         Tree.c
//
// Description :  Guarantee initialization of occupancy and root. It also
//                initializes the tree_count using a static counter.
//==========================================================================
template <class Whatever>
Tree<Whatever> ::Tree(void) : occupancy(0), root(NULL)

{
    // initialize tree_count
    tree_count = ++counter;
    // for debug message
    if (Tree<Whatever>::debug) {
        cerr << TREE << tree_count << ALLOCATE;
    }
}

//==========================================================================
// Routine Name : Tree<Whatever> :: ~Tree  (public)
// File :         Tree.c
//
// Description :  deallocates memory associated with the Tree.  It
//                will also delete all the memory of the elements within
//                the table.
//==========================================================================
template <class Whatever>
Tree<Whatever> :: ~Tree(void)

{
    // for debug message
    if (Tree<Whatever>::debug) {
        cerr << TREE << tree_count << DEALLOCATE;
    }
    // if root is not null
    if (root) {
        // call delete_AllTNodes
        root->delete_AllTNodes();
    }
    tree_count = --counter;
}

//==========================================================================
// Routine Name : Tree<Whatever> :: Write (public)
// File :         Tree.c
//
// Description : This function will output the contents of the Tree table
//               to the stream specificed by the caller.  The stream could be
//               cerr, cout, or any other valid stream.
//
// Parameters descriptions :
//
// name               description
// ------------------ ------------------------------------------------------
// stream             A reference to the output stream.
// <return>           A reference to the output stream.
//==========================================================================
template <class Whatever>
ostream& Tree<Whatever> ::Write(ostream& stream) const
{
    stream << "Tree " << tree_count << ":\n"
        << "occupancy is " << occupancy << " elements.\n";

    return (root) ? root->Write_AllTNodes(stream) : stream;
}
