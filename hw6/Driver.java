//==========================================================================
// cs12wi20cu                         Homework 6              Tianling Luo
//--------------------------------------------------------------------------
// File: Driver.java
//
// Description: In this assignment, I will write the class implementation
// for UCSDStudent which includes all UCSDStudent methods that will be 
// called by the HashTable code.
//==========================================================================

    import java.io.EOFException;  
      
//=========================================================================
// class UCSDStudent
//
// Description: The UCSDStudent class is defined and it contains the member 
// functions toString, UCSDStudent, getName, equals and hashCode to
// be called by the HashTable.java code.
//
// Data Fields:
//     name (String)  - the name of UCSD student
//     Studentnum (long)  - the student number of UCSD student
//
// Public functions:
//     toString - get the name and student number in string
//     UCSDStudent - store the name and student number in name and
//     		     Studentnum with input String and long
//     UCSDStudent - store the name and student number in name and
//     		     Studentnum with input of UCSDStudent type
//     getName - get the name of the student
//     equals - check if a student name is equal to another one
//     hashCode - get the ASCII sum of the student for a numeric value
//
//==========================================================================  
class UCSDStudent extends Base {
        private String name;    //name of student
        private long Studentnum;   //student number of student

        public String toString () {
		        //return the string with name and student number
                return "name: " + name + " Studentnum: " + Studentnum;
        }

	    public UCSDStudent (String nm, long val) {  
                //assign the name
		        name = new String (nm);  
		        //assign the Studentnum
                Studentnum = val;  
        }  
        public UCSDStudent (UCSDStudent ucsdstu) { 
	            //assign the name	
                name = new String (ucsdstu.name);
		        //assign the Studentnum
                Studentnum = ucsdstu.Studentnum;
        }  

        public String getName () {  
		        //return the name
                return name;  
        }  
        public boolean equals (Object other) {  
                if (this == other)   
                        return true;  
                  
                if (!(other instanceof UCSDStudent))   
                        return false;  
                  
                UCSDStudent otherVar = (UCSDStudent)other;  
                
		        //check if the names are the same  
                return name.equals (otherVar.getName ());  
        }  
          
        public int hashCode () {  
                int retval = 0;  
                int index = 0;  

		        //use a loop to sum up the ASCII value
                while (index != name.length ()) { 
		                //add the ASCII value	
                        retval += name.charAt (index); 
		                //increment the index	
                        index ++;  
                }  
  
                return retval;  
        }  
  
}

public class Driver {

        public static void main (String [] args) {

                /* initialize debug states */
                HashTable.debugOff();

                /* check command line options */
                for (int index = 0; index < args.length; ++index) {
                        if (args[index].equals("-x"))
                                HashTable.debugOn();
                }

                /* The real start of the code */
                SymTab symtab = new SymTab (5);
                String buffer = null;
                char command;
                long number = 0;

                System.out.print ("Initial Symbol Table:\n" + symtab);

                while (true) {
                        command = 0;    // reset command each time in loop
                        System.out.print ("Please enter a command:  "
                                + "((i)nsert, (l)ookup, (w)rite):  ");

                        try {
                        command = MyLib.getchar ();
                        MyLib.clrbuf (command); // get rid of return

                        switch (command) {
                        case 'i':
                                System.out.print (
                                "Please enter UCSD Student name to insert:  ");
                                buffer = MyLib.getline ();// formatted input

                                System.out.print (
                                        "Please enter UCSD Student number:  ");

                                number = MyLib.decin ();
                                MyLib.clrbuf (command); // get rid of return

                                // create Student and place in symbol table
                                if(!symtab.insert (
                                        new UCSDStudent (buffer, number), 1)){

                                        System.out.println("Couldn't insert " + 
                                                           "student!!!"); 
                                }
                                break;

                        case 'l': {
                                Base found;     // whether found or not

                                System.out.print (
                                "Please enter UCSD Student name to lookup:  ");

                                buffer = MyLib.getline ();// formatted input

                                UCSDStudent stu = new UCSDStudent (buffer, 0);
                                found = symtab.lookup (stu);
                                
                                if (found != null) {
                                        System.out.println ("Student found!!!");
                                        System.out.println (found);
                                }
                                else
                                        System.out.println ("Student " + buffer
                                                + " not there!");
                                }
                                break;

                        case 'w':
                                System.out.print (
                                    "The Symbol Table contains:\n" + symtab);
                        }
                        }
                        catch (EOFException eof) {
                                break;
                        }
                }

                System.out.print ("\nFinal Symbol Table:\n" + symtab);
        }
}
