/***************************************************************************
// cs12wi20cu                         Homework 7              Tianling Luo
//--------------------------------------------------------------------------
// File: Tree.java
//
// Description:  In the assignment, we will implement a binary tree data 
// structure. This structure will be used to allow variable assignment in 
// the calculator.
***************************************************************************/


public class Tree<Whatever extends Base> extends Base {  
      
        /* data fields */  
        private TNode root;  
        private long occupancy;   
        private String treeName;  
      
        /* debug flag */  
        private static boolean debug;  
      
        /* debug messages */  
        private static final String ALLOCATE = " - Allocating]\n";  
        private static final String AND = " and ";  
        private static final String COMPARE = " - Comparing ";  
        private static final String INSERT = " - Inserting ";  
        private static final String TREE = "[Tree ";  
     
        /** 
         * This method allocates and initializes the memory 
         * associated with a tree. 
         * 
         * @param  name   a String displayed when writing the Tree.
         */  
        public Tree (String name) {  
                /* initialize */ 
                treeName=name;
                occupancy=0;
                root=null;

                /* for debugging message */
                if(debug){
                    System.err.print(TREE+treeName+ALLOCATE);        
				}
        }  

        /** 
         * This method will turn off debugging for this HashTable.
         * 
         * @param   None.
         * @return  No return value. Turn off debugging.
         */   
        public static void debugOff () {  
                /* turn of debugging */
                debug=false;
        }  

        /** 
         * This method will turn on debugging for this HashTable.
         * 
         * @param   None.
         * @return  No return value. Turn on debugging.
         */   
        public static void debugOn () {  
                /* turn on debugging */  
                debug=true;
        }  
      
        /** 
         * Returns the tree's name 
         * @return name of the tree 
         */  
        public String getName() {  
                return treeName;  
        }  
      
        /** 
         * This method will insert the element in the binary tree. 
         * If the element cannot be inserted, false will be returned. 
         * If the element can be inserted, the element is inserted 
         * and true is returned.  Duplicate insertions will 
         * cause the existing element to be deleted, and the duplicate 
         * element to take its place.
         * 
         * @param   element       The complete element to insert. 
         * @return  true or false indicating success or failure of 
         * insertion 
         */  
        public boolean insert (Whatever element) {  
                /* use working to track in the tree */ 
                TNode working=root;
                /* track left & right height */
                long left_height;
                long right_height;
                /* if inserting the first node */                
                if(root==null){
                    root=new TNode(element);
                    /* for debugging message */
                    if(debug){
                        System.err.print(TREE+treeName+INSERT
                            +element.getName()+"]\n");
				    }
                    return true;
                }

                else while(true){
                    /* for debugging message */
                    if(debug){
                        System.err.print(TREE+treeName+COMPARE
                            +element.getName()+AND
                            +working.data.getName()+"]\n");
				    }

                    /* if equal to current node */
                    if(element.equals(working.data)){
                        /* turn off the flag if it's on */
                        if(working.hasBeenDeleted){
                            working.hasBeenDeleted=false;
                            occupancy++;
                        }
                        /* replace for duplicate */
                        working.data=element;
                        break;
					}
                    /* if greater than current node */
                    else if (element.isGreaterThan(working.data)){
                        /* if right child does not exist */
                        if(working.right==null){
                            working.right=new TNode(element);
                            working.right.parent=working;
                            break;
                        }
                        /* if right child exists */
                        else
                            working=working.right;
					}
                    /* if less than current node */
                    else{
                        /* if left child does not exist */
                        if(working.left==null){
                            working.left=new TNode(element);
                            working.left.parent=working;
                            break;
                        }
                        /* if left child exists */
                        else
                            working=working.left;
					}
				}

                /* for debugging message */
                if(debug){
                    System.err.print(TREE+treeName+INSERT
                            +element.getName()+"]\n");        
				}

                /* loop of going up the tree */
                while(working!=null){
                    /* initialze for null */
                    left_height=-1;
                    right_height=-1;
                    working.height=0;
                    working.balance=0;
                    /* if left child exists */
                    if(working.left!=null)
                        left_height=working.left.height;
                    
                    /* if right child exists */
                    if(working.right!=null)
                        right_height=working.right.height;
                    
                    /* if either left or right child exists */
                    if (working.left!=null || working.right!=null){
                        working.height=(left_height>=right_height)?
                        (left_height+1):(right_height+1);
                        working.balance=left_height-right_height;
                    }
                    /* go up the tree */
                    working=working.parent;
				}
                
                return true;  
        }  
      
        /** 
         * This method will lookup the element in the binary tree. If 
         * found a pointer to the element is returned. If the element 
         * is not found, NULL will be returned to the user. 
         * 
         * @param   element  The element to look up. 
         * @return  A pointer to the data if found, else null 
         */  
        public Whatever lookup (Whatever element) {  
                /* use working to track in the tree */ 
                TNode working=root;
                
                /* if root is null */
                if(root==null){
                    return null;
                }

                else while(true){
                    /* for debugging message */
                    if(debug){
                        System.err.print(TREE+treeName+COMPARE
                            +element.getName()+AND
                            +working.data.getName()+"]\n");
				    }
                    
                    /* if equal to current node */
                    if(element.equals(working.data)){
                        /* check if the flag is on */
                        if(working.hasBeenDeleted)
                            break;
                        else 
                            return working.data;
					}
                    /* if greater than current node */
                    else if (element.isGreaterThan(working.data)){
                        /* if right child does not exist */
                        if(working.right==null){
                            break;
                        }
                        /* if right child exists */
                        else
                            working=working.right;
					}
                    /* if less than current node */
                    else{
                        /* if left child does not exist */
                        if(working.left==null){
                            break;
                        }
                        /* if left child exists */
                        else
                            working=working.left;
					}
				}
                return null;    /* not there */
        }  
      
        /** 
         * This method will remove the element in the tree.  If 
         * found a pointer to the element is returned.  If the element 
         * is not found, NULL will be returned to the user. 
         * 
         * @param   element  The complete element to remove. 
         * @return  A pointer to the data if found, else NULL 
         */  
        public Whatever remove (Whatever element) {  
                /* use working to track in the tree */ 
                TNode working=root;
                
                /* if root is null */
                if(root==null){
                    return null;
                }

                else while(true){
                    /* for debugging message */
                    if(debug){
                        System.err.print(TREE+treeName+COMPARE
                            +element.getName()+AND
                            +working.data.getName()+"]\n");
				    }

                    /* if equal to current node */
                    if(element.equals(working.data)){
                        /* turn on the flag if it's on */
			if(!working.hasBeenDeleted){
				working.hasBeenDeleted=true;
				occupancy--;
				return working.data;
			}
			else
				break;
		    }
                    /* if greater than current node */
                    else if (element.isGreaterThan(working.data)){
                        /* if right child does not exist */
                        if(working.right==null){
                            break;
                        }
                        /* if right child exists */
                        else
                            working=working.right;
					}
                    /* if less than current node */
                    else{
                        /* if left child does not exist */
                        if(working.left==null){
                            break;
                        }
                        /* if left child exists */
                        else
                            working=working.left;
					}
				}
                return null;    /* not there */ 
        }  
      
        /** 
         * Creates a string representation of this tree. This method first 
         * adds the general information of this tree, then calls the 
         * recursive TNode function to add all nodes to the return string  
         * 
         * @return  String representation of this tree  
         */  
        public String toString () {  
                String string = "Tree " + treeName + ":\noccupancy is ";  
                string += occupancy + " elements.";  
    
                if(root != null)  
                        string += root.writeAllTNodes();  
      
                return string;  
        }  

/***************************************************************************
// class TNode
//
// Description: Nodes of the binary tree are represented by TNode objects.
// Each TNode contains the data stored in the TNode, height and balance
// information, and references to left and right children, as well as to
// the parent TNode. Each TNode of the Tree will have a hasBeenDeleted
// field.
//
// Data Fields:
//     data (Whatever)  - data to store in TNode
//     left (TNode)  - reference to left children
//     right (TNode)  - reference to right children
//     parent (TNode)  - reference to parent TNode
//     hasBeenDeleted (boolean)  - track if deleted
//     balance (long) - balance of the node
//     height (long) - height of the node
//
// Public functions:
//     TNode - constructor
//     toString - Creates a string representation of this node
//     writeAllTNodes - Writes all TNodes to the String representation 
//          field.
***************************************************************************/
        private class TNode {  
      
                public Whatever data;  
                public TNode left, right, parent;  
                public boolean hasBeenDeleted;  
      
                /* left child's height - right child's height */  
                public long balance;  
                /* 1 + height of tallest child, or 0 for leaf */  
                public long height;  
                
                /** 
                 * This method is the constructor of TNode. It
                 * allocates and initializes the memory associated 
                 * with the TNode object. 
                 * 
                 * @param   element  The data stored in the TNode. 
                 */  
                public TNode (Whatever element) {
                        /* allocate and initialize the memory */
                        data=element;
                        height=0;
                        balance=0;
                        left=null;
                        right=null;
                        parent=null;
                        hasBeenDeleted=false;
                        /* increment on occupancy */
                        occupancy++;
                }  
      
                /** 
                 * Creates a string representation of this node. Information 
                 * to be printed includes this node's height, its balance, 
                 * and the data its storing. 
                 * 
                 * @return  String representation of this node  
                 */  
                public String toString () {  
                        return "at height:  " + height + "  with balance: " +  
                                balance + "  " + data;  
                }  
      
                /** 
                 * Writes all TNodes to the String representation field.  
                 * This recursive method performs an in-order 
                 * traversal of the entire tree to print all nodes in 
                 * sorted order, as determined by the keys stored in each 
                 * node. To print itself, the current node will append to 
                 * tree's String field. 
                 */  
                public String writeAllTNodes () {  
                        String string = "";  
                        if (left != null)  
                                string += left.writeAllTNodes ();  
                        if (!hasBeenDeleted)   
                                string += "\n" + this;            
                        if (right != null)  
                                string += right.writeAllTNodes ();  
      
                        return string;  
                }  
        }  
}  
