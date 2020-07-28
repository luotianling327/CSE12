//==========================================================================
// cs12wi20cu                         Homework 9              Tianling Luo
//--------------------------------------------------------------------------
// File: Tree.c
//
// Description: In this assignment, we will implement a binary tree data
// structure to use the "template" feature of C++. 
//==========================================================================

#include <stdlib.h>
#include <string.h>
#include "Tree.h"

// debug messages
static const char ALLOCATE[] = " - Allocating]\n";
static const char COST_READ[] = "[Cost Increment (Disk Access): Reading ";
static const char COST_WRITE[] = "[Cost Increment (Disk Access): Writing ";
static const char DEALLOCATE[] = " - Deallocating]\n";
static const char TREE[] = "[Tree ";

//=========================================================================
// class Tree
//
// Description: The Tree class is defined and it contains the member
// functions that will be called by the Tree.c code.
//
// Data Fields:
//     debug_on (static int) - option to set debug message 
//     operation (static long) - 
//     TNode<Whatever> (friend struct) - the tree node
//     cost (static long) - 
//     fio (fstream*) - 
//     occupancy (long) - the occupancy in the tree
//     root (offset) - the root of the tree
//     tree_count (unsigned long) - number of trees
//     ResetRoot (void) - 
//
// Public functions:
//     Tree - constructor
//     ~Tree - destructor
//     static void Set_Debug_On - set debug on
//     static void Set_Debug_Off - set debug off
//     long GetCost - get the cost
//     long GetOperation - get the operation
//     void IncrementCost - increment the cost
//     void IncrementOperation - increment the operation
//     void ResetRoot - 
//     unsigned long Insert - Insert nodes into the tree
//     unsigned long Remove - Remove nodes from the tree
//     unsigned long Lookup - Look up nodes in the tree
//     ostream& Write - Write in in-order traversal
//========================================================================== 

template <class Whatever>
int Tree<Whatever>::debug_on = 0;

template <class Whatever>
long Tree<Whatever>::cost = 0;

template <class Whatever>
long Tree<Whatever>::operation = 0;

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#define THRESHOLD 2

template <class Whatever>
ostream& operator << (ostream&, const TNode<Whatever>&);

//=========================================================================
// struct TNode
//
// Description: The TNode structure is defined and it contains the member
// functions that will be called by the Tree.c code.
//
// Data Fields:
//     data (Whatever)  - data to store in TNode
//     balance (long)  - balance of the node
//     height (long) - height of the node
//     left (offset) - offset to the left
//     right (offset) - offset to the right
//     this_position (offset) - current position
//
// Public functions:
//     TNode - TNode read constructor
//     TNode - TNode write constructor
//     unsigned long Insert - insert nodes
//     unsigned long Lookup - look up the nodes
//     void Read - read node from the disk
//     unsigned long Remove - remove nodes
//     void ReplaceAndRemoveMax - replace TNode with max TNode in left
//     subtree
//     void SetHeightAndBalance - Update height and balance
//     void Write - Write in in-order traversal
//     ostream& Write_AllTNodes - to write all TNodes
//==========================================================================  
template <class Whatever>
struct TNode {
	// friends:

	// data fields:
	Whatever data;
	long height;
	long balance;
	offset left;
	offset right;
	offset this_position;	// current position

	// function fields:
	TNode() : height(0), balance(0), left(0), right(0),
		this_position(0) {}

	// to declare the working TNode in Tree's Remove
	TNode(Whatever& element) : data(element), height(0), balance(0),
		left(0), right(0), this_position(0) {}

	TNode(Whatever&, fstream*, long&);	// to add new node to disk
	TNode(const offset&, fstream*);	// to read node from disk

	unsigned long Insert(Whatever&, fstream*, long&, offset&);
	// optional recursive Lookup declaration would go here
	unsigned long Lookup(Whatever&, fstream*) const;
	void Read(const offset&, fstream*);	// read node from disk
	unsigned long Remove(TNode<Whatever>&, fstream*, long&, offset&,
		long fromSHB = FALSE);
	void ReplaceAndRemoveMax(TNode<Whatever>&, fstream*, offset&);
	void SetHeightAndBalance(fstream*, offset&);
	void Write(fstream*) const;		// update node to disk

	ostream& Write_AllTNodes(ostream&, fstream*) const;
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
    debug_on = 1;
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
    debug_on = 0;
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
unsigned long Tree<Whatever> ::Insert(Whatever& element) {
	// increment operation counter
	operation++;
	// if root is null
	if (occupancy == 0) {
		TNode<Whatever> rootTNode(element, fio,occupancy);
		root=rootTNode.this_position;
		return 1;
	}
	// if root exists
	else {
		// ensure to have access to root node
		TNode<Whatever> rootNode(root, fio);
		// call insert with respect to the node
		return(rootNode.Insert(element,fio,occupancy, root));
	}
}

//==========================================================================
// Routine Name : TNode<Whatever> ::ReplaceAndRemoveMax
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
// targetTNode        a reference to the TNode to remove that has 2children
// fio  filestream corresponding to the datafile where the
//		      Tree is stored on disk
// PositionInParent   a reference to the TNode position in the parent TNode 
//                    used to get to the current TNode
//==========================================================================
template <class Whatever>
void TNode<Whatever> ::ReplaceAndRemoveMax(TNode<Whatever>& targetTNode,
	fstream* fio, offset& PositionInParent) {
	// if right exists
	if (right) {
		// call to right recursively
		TNode<Whatever> rightTNode(right, fio);
		rightTNode.ReplaceAndRemoveMax(targetTNode, fio, right);
		// set height and balance
		SetHeightAndBalance(fio, PositionInParent);
	}
	// if right does not exist
	else {
		PositionInParent = left;
		// reassign the data to targetTNode
		targetTNode.data = data;
	}
}

//==========================================================================
// Routine Name : TNode<Whatever> ::Remove
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
// fio	filestream corresponding to the datafile where the
//		      Tree is stored on disk
// occupancy	      reference to the occupancy of the tree
// PositionInParent   a reference to the TNode position in the parent TNode
//                    used to get to the current TNode
// fromSHB            true or false, keeping track of whether or not Remove
//                    was called from SetHeightAndBalance so that Remove
//                    can determine whether to call SetHeightAndBalance
// <return>           Returns true or false indicating success of removal
//==========================================================================
template <class Whatever>
unsigned long TNode<Whatever> ::Remove(TNode<Whatever>& elementTNode,
	fstream* fio, long& occupancy, offset& PositionInParent,
	long fromSHB) {
	// track the return value
	long status = 0;
	// calls == on data to compare names
	if (elementTNode.data == data) {
		// update the data
		elementTNode.data = data;
		status = 1;
		// if only left child exists
		if (left && !right) {
			// PIP is left
			PositionInParent = left;
		}
		// if only right child exists
		else if (right && !left) {
			// PIP is right
			PositionInParent = right;
		}
		// if with no child
		else if (!right && !left) {
			// PIP is null
			PositionInParent = 0;
		}
		// if with two children
		else {
			// call ReplaceAndRemoveMax
			TNode<Whatever> lTNode(left, fio);
			lTNode.ReplaceAndRemoveMax(*this, fio, left);
		}
			// determine to call SetHeightAndBalance or not
			if (!fromSHB) {
				SetHeightAndBalance(fio, PositionInParent);
			}
			else{
				// need to write the remove result to file
				Write(fio);
			}
	}
	// go right
	else if (elementTNode.data > data) {
		// if right is not null
		if (right) {
			// call recursively
			TNode<Whatever> rightTNode(right, fio);
			status = rightTNode.Remove(elementTNode,
	fio, occupancy, right,fromSHB);
			}
	}
	//go left
	else {
		// if left is not null
		if (left) {
			// call recursively
			TNode<Whatever> leftTNode(left, fio);
			status = leftTNode.Remove(elementTNode,
	fio, occupancy, left,fromSHB);
		}
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
	// increment operation
	operation++;
	// to store the return value
	long status = 0;
	// if occupancy is 0
	if (occupancy==0)
		return 0;
	else if (occupancy==1){
		// create for remove
		TNode<Whatever> elementTNode(element);
		// delegate to TNode's Remove
		TNode<Whatever> rootTNode(root, fio);
		status = rootTNode.Remove(elementTNode, fio, 
				occupancy, root,false);
		// update element
		element = elementTNode.data;
		// call reset root
		ResetRoot();
		// decrement on occupancy
		occupancy--;
	}
	else {
		// create for remove
		TNode<Whatever> elementTNode(element);
		// delegate to TNode's Remove
		TNode<Whatever> rootTNode(root, fio);
		status = rootTNode.Remove(elementTNode, fio, 
				occupancy, root,false);
		// update element
		element = elementTNode.data;
		// decrement on occupancy
		occupancy--;
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
// fio	filestream corresponding to the datafile where the 
//		      Tree is stored on disk
// PositionInParent   a reference to the TNode position in the parent TNode
//                    used to get to the current TNode.
//==========================================================================
template <class Whatever>
void TNode<Whatever> ::SetHeightAndBalance(fstream* fio,
	offset& PositionInParent) {
	// intialize left and right height
	long leftHeight = -1;
	long rightHeight = -1;
	// if left exists
	if (left) {
		TNode<Whatever> leftTNode(left, fio);
		leftHeight = leftTNode.height;
	}
	// if right exists
	if (right) {
		TNode<Whatever> rightTNode(right, fio);
		rightHeight = rightTNode.height;
	}
	// set height
	height = (leftHeight >= rightHeight) ?
		(leftHeight + 1) : (rightHeight + 1);
	// set balance
	balance = leftHeight - rightHeight;
	// if is unbalanced
	if (abs(balance) > THRESHOLD) {
		// pass in a fakePccupancy
		long fakeOccupancy=0;
		TNode<Whatever> elementTNode(data);
		// remove
		Remove(elementTNode, fio, fakeOccupancy,
			       	PositionInParent, true);
		// reinsert
		TNode<Whatever> PIPTNode(PositionInParent, fio);
		PIPTNode.Insert(elementTNode.data, fio,
			fakeOccupancy, PositionInParent);
	}
	// if not out of balance
	else {
		// write it to the file
		Write(fio);
	}
}

//==========================================================================
// Routine Name : Tree<Whatever> ::GetCost
// File :         Tree.c
//
// Description : Returns the value of the Tree<Whatever>::cost variable.
//
// Parameters descriptions :
//
// name               description
// ------------------ ------------------------------------------------------
// <return>           Returns the value of Tree<Whatever>::cost variable.
//==========================================================================
template <class Whatever>
long Tree <Whatever> ::GetCost() {
	// returns the value of the Tree<Whatever>::cost variable
	return cost;
}

//==========================================================================
// Routine Name : Tree<Whatever> ::GetOperation
// File :         Tree.c
//
// Description : Returns the value of the Tree<Whatever>::operation variable
//
// Parameters descriptions :
//
// name               description
// ------------------ ------------------------------------------------------
// <return>           Returns value of Tree<Whatever>::operation variable
//==========================================================================
template <class Whatever>
long Tree <Whatever> ::GetOperation() {
	// returns the value of the Tree<Whatever>::operation variable
	return operation;
}

//==========================================================================
// Routine Name : Tree<Whatever> ::IncrementCost
// File :         Tree.c
//
// Description : Increments the value of the Tree<Whatever>::cost variable.
// This should be called when a read or write to disk occurs. 
//==========================================================================
template <class Whatever>
void Tree <Whatever> ::IncrementCost() {
	// increments the value of the Tree<Whatever>::cost variable
	cost++;
}

//==========================================================================
// Routine Name : Tree<Whatever> ::IncrementOperation
// File :         Tree.c
//
// Description : increments the value of the Tree<Whatever>::operation
// variable. This should be called when a Tree operation occurs (insert,
// lookup, remove)
//==========================================================================
template <class Whatever>
void Tree <Whatever> ::IncrementOperation() {
	// increments the value of the Tree<Whatever>::operation variable
	operation++;
}

//==========================================================================
// Routine Name : Tree<Whatever> ::ResetRoot
// File :         Tree.c
//
// Description : Resets the root datafield of this tree to be at the end of
// the datafile. This should be called when the last TNode has been removed
// from the Tree. 
//==========================================================================
template <class Whatever>
void Tree <Whatever> ::ResetRoot() {
	// move the root field to print to the end of the file
	fio->seekp(0, ios::end);
	root = fio->tellp();
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
// fio  filestream corresponding to the datafile where the
//		      Tree is stored on disk
// occupancy	      reference to the occupancy of the tree
// PositionInParent   a reference to the TNode position in the parent TNode
//                    used to get to the current TNode.
// <return>           true or false indicating success of insertion
//==========================================================================
template <class Whatever>
unsigned long TNode<Whatever> ::Insert(Whatever& element, fstream* fio,
	long& occupancy, offset& PositionInParent) {
	// duplicate case
	if (element == data) {
		data = element;
		// write
		Write(fio);
		return 1;
	}
	//check to go right
	else if (element > data) {
		// if right child doesn't exist
		if (!right) {
			// create TNode
			TNode<Whatever> rChild(element, fio, occupancy);
			right = rChild.this_position;
		}
		else{
			// recursion on the right
			TNode<Whatever> rightTNode(right, fio);
			rightTNode.Insert(element, fio, occupancy, right);
		}
	}
	// go left
	else {
		// if left child doesn't exist
		if (!left) {
			// create TNode
			TNode<Whatever> lChild(element, fio, occupancy);
			left = lChild.this_position;
		}
		else{
			// recursion on the right
			TNode<Whatever> leftTNode(left, fio);
			leftTNode.Insert(element, fio, occupancy, left);
		}
	}
	// set height and balance
	SetHeightAndBalance(fio,PositionInParent);
	return 1;
}

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
template <class Whatever>
unsigned long TNode<Whatever> ::Lookup(Whatever& element,fstream* fio) const
{
	// store the return value
	long status = 0;
	if (element == data) {
		// update the data
		element = data;
		status = 1;
	}
	// check to go right
	else if (element > data) {
		// if right exists
		if (right) {
			// recursion on the right
			TNode<Whatever> rightTNode
				(right, fio);
			status = rightTNode.Lookup(element,fio);
		}
	}
	// go left
	else {
		// if left exists
		if (left) {
			// recursion on the left
			TNode<Whatever> leftTNode
				(left, fio);
			status = leftTNode.Lookup(element,fio);
		}
	}
	return status;
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
	// increment operation
	operation++;
	// to store the return value
	long status = 0;
	// if root is null
	if (!root)
		return 0;
	// root is not null
	else {
		// call TNode's Lookup
		TNode<Whatever> rootTNode(root, fio);
		status = rootTNode.Lookup(element,fio);
	}
	return status;
}

//==========================================================================
// Routine Name : Read
// File :         Tree.c
//
// Description :  Reads a TNode which is present on the datafile into 
// memory. The TNode is read from position. The TNode's information in the
// datafile overwrites this TNode's data. 
//
// Parameters descriptions :
//
// name               description
// ------------------ ------------------------------------------------------
// position	      offset in the datafile corresponding to the position
// of the TNode we wish to read into memory.
// fio    filestream corresponding to the datafile where the
//		      Tree is stored on disk
//==========================================================================
template <class Whatever>
void TNode<Whatever> ::Read(const offset& position, fstream* fio) {
	// go to the position
	fio->seekg(position);
	// read into current memory location
	fio->read(((char*) this), sizeof(TNode<Whatever>));
	// debug message
	if(Tree<Whatever>::debug_on){
		cerr<<COST_READ<<(const char*)(data)<<"]"<<endl;
	}
	// increment the cost
	Tree <Whatever> ::IncrementCost();
}

//==========================================================================
// Routine Name : TNode
// File :         Tree.c
//
// Description :  Called when reading a TNode present on disk into memory
//
// Parameters descriptions :
//
// name               description
// ------------------ ------------------------------------------------------
// position           offset in the datafile corresponding to the position
// of the TNode we wish to read into memory.
// fio    filestream corresponding to the datafile where the
//		      Tree is stored on disk
//==========================================================================
template <class Whatever>
TNode<Whatever> ::TNode(const offset& position, fstream* fio) {
	// call the Read method
	Read(position,fio);
}

//==========================================================================
// Routine Name : TNode
// File :         Tree.c
//
// Description :  Called when creating a TNode for the first time
//
// Parameters descriptions :
//
// name               description
// ------------------ ------------------------------------------------------
// element            the data to be stored in the TNode
// fio	      filestream corresponding to the datafile where the
//		      Tree is stored on disk
// occupancy    a reference to the occupancy of the tree to wich the
//		      new Node is being added
//==========================================================================
template <class Whatever>
TNode<Whatever> ::TNode(Whatever& element, fstream* fio, long& occupancy) :
	data(element), height(0), balance(0), left(0),
	right(0) {
	// increment occupancy
	occupancy++;
	// seek the write pointer to the end of the file
	fio->seekp(0, ios::end);
	// set this_position corresponding to the end of the file
	this_position = fio->tellp();
	// call Write
	Write(fio);
}

//==========================================================================
// Routine Name : Write
// File :         Tree.c
//
// Description :  Writes this TNode object to disk at this_position in the
//				  datafile. 
//
// Parameters descriptions :
//
// name               description
// ------------------ ------------------------------------------------------
// fio   filestream corresponding to the datafile where the
//		      Tree is stored on disk
//==========================================================================
template <class Whatever>
void TNode<Whatever> ::Write(fstream* fio) const {
	// go to current position
	fio->seekp(this_position);
	// write it out to the file
	fio->write(((const char*) this), sizeof(TNode<Whatever>));
	// debug message
	if(Tree<Whatever>::debug_on){
		cerr<<COST_WRITE<<(const char*)(data)<<"]"<<endl;
	}
	// increment the cost
	Tree <Whatever> ::IncrementCost();
}

//==========================================================================
// Routine Name : Tree<Whatever> :: Tree  (public)
// File :         Tree.c
//
// Description :  Allocates the tree object. Checks the datafile to see if
// it contains Tree data. If it is empty, root and occupancy fields are
// written to the file. If there is data in the datafile, root and occupancy
// fields are read into memory. 
//==========================================================================
template <class Whatever>
Tree<Whatever> ::Tree(const char* datafile) :
	fio(new fstream(datafile, ios::out | ios::in)),
	occupancy(0),root(0){
	// counter for tree
	static long counter;
	// increment on tree count
	tree_count=++counter;
	// for debug message
	if (debug_on){
		cerr<<TREE<<tree_count<<ALLOCATE;
	}
	// check for a empty file
	fio->seekg(0, ios::beg);
	offset begin = fio->tellg();
	fio->seekg(0, ios::end);
	offset ending = fio->tellg();
	// file is empty
	if (begin == ending) {
		// write root and occypancy to file
		fio->seekp(0, ios::beg);
		fio->write((const char*)&root, sizeof(root));
		fio->write((const char*)&occupancy, sizeof(occupancy));
		root = fio->tellp();
	}
	// the disk has contents
	else {
		// read into memory
		fio->seekg(0, ios::beg);
		fio->read((char*)&root, sizeof(root));
		fio->read((char*)&occupancy, sizeof(occupancy));
	}
}

template <class Whatever>
Tree<Whatever> :: ~Tree(void)
//==========================================================================
// Routine Name : Tree :: ~Tree  (public)
// File :         Tree.c
//
// Description :  deallocates memory associated with the Tree.  It
//                will also delete all the memory of the elements within
//                the table.
//==========================================================================

{
	// for debug message
	if (debug_on){
		cerr<<TREE<<tree_count<<DEALLOCATE;
	}
	// write root and occypancy to file
	fio->seekp(0, ios::beg);
	fio->write((const char*)&root, sizeof(root));
	fio->write((const char*)&occupancy, sizeof(occupancy));
	// decrement on tree_count
	tree_count--;
	// delete the fstream pointer
	delete fio;
}	// end: ~Tree

template <class Whatever>
ostream& operator << (ostream& stream, const TNode<Whatever>& nnn) {
	stream << "at height:  :" << nnn.height << " with balance:  "
		<< nnn.balance << "  ";
	return stream << nnn.data << "\n";
}

template <class Whatever>
ostream& Tree<Whatever> ::Write(ostream& stream) const
//==========================================================================
// Routine Name : Tree :: Write (public)
// File :         Tree.c
//
// Description : This funtion will output the contents of the Tree table
//               to the stream specificed by the caller.  The stream could
//               be cerr, cout, or any other valid stream.
//
// Parameters descriptions :
//
// name               description
// ------------------ ------------------------------------------------------
// stream             A reference to the output stream.
// <return>           A reference to the output stream.
//==========================================================================

{
	long old_cost = cost;

	stream << "Tree " << tree_count << ":\n"
		<< "occupancy is " << occupancy << " elements.\n";

	fio->seekg(0, ios::end);
	offset end = fio->tellg();

	// check for new file
	if (root != end) {
		TNode<Whatever> readRootNode(root, fio);
		readRootNode.Write_AllTNodes(stream, fio);
	}

	// ignore cost when displaying nodes to users
	cost = old_cost;

	return stream;
}

template <class Whatever>
ostream& TNode<Whatever> ::
Write_AllTNodes(ostream& stream, fstream* fio) const {
	if (left) {
		TNode<Whatever> readLeftNode(left, fio);
		readLeftNode.Write_AllTNodes(stream, fio);
	}
	stream << *this;
	if (right) {
		TNode<Whatever> readRightNode(right, fio);
		readRightNode.Write_AllTNodes(stream, fio);
	}

	return stream;
}
