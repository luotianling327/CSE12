//==========================================================================
// cs12wi20cu                         Homework 6              Tianling Luo
//--------------------------------------------------------------------------
// File: HashTable.java
//
// Description: In the assignment, I will implement a Symbol Table to
// allow variable assignment to the calculator. 
//==========================================================================

    public class HashTable extends Base {  
      
            private static int counter = 0;     // number of HashTables so far  
            private static boolean debug;        // allocation of debug state  
      
            // set in locate, last location checked in hash table   
            private int index = 0;   
      
            // set in insert/lookup, count of location in probe sequence  
            private int count = 0;  
              
            private int probeCount[];   // where we are in the probe sequence   
            private long occupancy;     // how many elements are in the Hash Table  
            private int size;           // size of Hash Table  
            private Base table[];       // the Hash Table itself ==> array of Base  
            private int tableCount;     // which hash table it is  
      
            // messages  
            private static final String AND = " and ";  
            private static final String DEBUG_ALLOCATE = " - Allocated]\n";  
            private static final String DEBUG_LOCATE = " - Locate]\n";  
            private static final String DEBUG_LOOKUP = " - Lookup]\n";  
            private static final String BUMP = "[Bumping To Next Location...]\n";  
            private static final String COMPARE = " - Comparing ";  
            private static final String FOUND_SPOT = " - Found Empty Spot]\n";  
            private static final String HASH = "[Hash Table ";  
	    private static final String HASH_VAL = "[Hash Value Is ";  
            private static final String INSERT = " - Inserting ";  
            private static final String PROCESSING = "[Processing ";  
            private static final String TRYING = "[Trying Index ";  
      
              
            /** 
             * This method will turn on debugging for this HashTable.
             * 
             * @param   None.
             * @return  No return value. Turn on debugging.
             */   
            public static void debugOn () {  
                    // set debug to be true
                    debug=true;
                    return;
            }  
      
            /** 
             * This method will turn off debugging for this HashTable.
             * 
             * @param   None.
             * @return  No return value. Turn off debugging.
             */   
            public static void debugOff () {  
                    // set debug to be false
                    debug=false;
                    return;
            }  
      
            /** 
             * This method allocates and initializes the memory 
             * associated with a hash table. 
             * 
             * @param  sz   The number of elements for the table...MUST BE PRIME!!! 
             */  
            public HashTable (int sz) {  
                    //use in the loop as counter
                    int iteration;
                    size=sz;
                    //allocates the table and probeCount
                    table=new Base[size];
		    probeCount=new int[size];
                    //initialize
                    occupancy=0;
                    tableCount=++counter;
                    //use a loop to intialize to null
                    for(iteration=0;iteration<size;iteration++){
                        table[iteration]=null;
		    }

		            //for debugging message
		            if(debug){
			            System.err.print(HASH+tableCount+DEBUG_ALLOCATE);
		            }
            }  
      
      
            /** 
             * This method will insert the element in the hash table. 
             * If the element cannot be inserted, false will be returned. 
             * If the element can be inserted, the element is inserted 
             * and true is returned.  Duplicate insertions will 
             * cause the existing element to be deleted, and the duplicate 
             * element to take its place. 
             * 
             * @param   element       The element to insert. 
             * @param   initialCount  Where to start in probe seq (recursive calls)  
             * @return  true or false indicating success or failure of insertion 
             */  
            public boolean insert (Base element, int initialCount) {  
                    //use temp to store the original element at index
                    Base temp;
                    //use temp_count to store the probe seq
                    int temp_count;
                    //set count field
                    count=initialCount;
                    //use locate to store the locate return
                    boolean locate;

                    //for debugging message
		            if(debug){
                        System.err.print(HASH+tableCount+INSERT
                        +element.getName()+"]\n");
			    }

                    //store the return of locate
                    locate=locate(element);
                    
                    //if locate returns true
                    if (locate){
			table[index]=element;
			probeCount[index]=count;
			if(occupancy<size)
				occupancy++;
				
                        return true;
		    }

                    //if the table is full
                    if(occupancy>=size){
			
			return false;	
		    }

                    //if locate returns false
                    if(!locate){
                        //if it's empty at index
                        if(table[index]==null){
                            //increment the occupancy
                            occupancy++;
                            table[index]=element;
                            probeCount[index]=count;
                            return true;
                        }
                        //if there already exists an item
                        else{
                            temp=table[index];
                            temp_count=probeCount[index];
                            table[index]=element;
			    probeCount[index]=count;

                            //for debugging message
                            if(debug){
                                System.err.print(BUMP);                 
			    }

                            //use recursion to bump
                            insert(temp,temp_count+1);
                            return true;
			}
		    }
		    else
			    return false;

            }  
      
      
            /** 
             * This method will locate the location in the 
             * table for the insert or lookup. 
             * 
             * @param   element  The element needing a location. 
             * @return  true if tem found, or false if not found 
             */  
            private boolean locate (Base element) {  
                    //get the numeric arrtibute 
                    int ASCII_Sum = element.hashCode();
                    //initial location
                    int Init = ASCII_Sum % size;
                    //increment
                    int increment=(ASCII_Sum%(size-1)+1);
		    //use index to store current location
                    index = (Init+(count-1)*increment)%size;

                    if(debug){
                        System.err.print(HASH+tableCount+DEBUG_LOCATE);
                        System.err.print(PROCESSING+element.getName()+"]\n");
                        System.err.print(HASH_VAL+ASCII_Sum+"]\n");
		    }
                    
		    //use loop to find the item
                    while(true){
                        //for debugging message
                        if (debug){
                            System.err.print(TRYING+index+"]\n");              
			}

                        //if finds an empty index
                        if(table[index]==null){
                            //for debug message
                            if(debug){
                                System.err.print(HASH+tableCount+FOUND_SPOT);                 
			    }
                            return false;
                        }

                        //for debug message
                        if (debug){
                            System.err.print(HASH+tableCount+COMPARE
                            +element.getName()+AND
                            +table[index].getName()+"]\n");  
			}

		        //if the element is found
                        if(table[index].equals(element))
                            return true;

                        //if the occupied index hasn't searched
                        //as long as the current element has
                        //searched
                        if(probeCount[index]<count)
                            return false;

                        //if we exhaust the table
                        if(count>size)
                            return false;

		       	//let index be next location index
                        index=(index+increment)%size;
                        //increment count
       	                count++;
					}
            }  
      
      
            /** 
             * This method will lookup the element in the hash table.  If 
             * found a pointer to the element is returned.  If the element 
             * is not found, NULL will be returned to the user. 
             * 
             * @param   element  The element to look up. 
             * @return  A pointer to the element if found, else NULL 
             */  
            public Base lookup (Base element) {  
                    //set count
		            count=1;
                    //use locate to store locate return
                    boolean locate;

                    //for debugging message
                    if(debug){
                        System.err.print(HASH+tableCount+DEBUG_LOOKUP);           
		    }

                    //store the return of locate
                    locate=locate(element);
		            //if locate returns true
		            if (locate){
			            return table[index];
		            }
		            //if locate returns false
		            else
			            return null;
            }  
      
      
            /** 
             * Creates a string representation of the hash table. The method  
             * traverses the entire table, adding elements one by one ordered 
             * according to their index in the table.  
             * 
             * @return  String representation of hash table 
             */  
            public String toString () {  
            String string = "Hash Table " + tableCount + ":\n";  
                    string += "size is " + size + " elements, ";   
                    string += "occupancy is " + occupancy + " elements.\n";  
      
                    /* go through all table elements */  
                    for (int index = 0; index < size; index++) {  
      
                            if (table[index] != null) {  
                                    string += "at index " + index + ": ";  
                                    string += "" + table[index];  
                                    string += " with probeCount: ";   
                                    string += probeCount[index] + "\n";  
                            }  
                    }  
      
                    return string;  
            }  
    }  
