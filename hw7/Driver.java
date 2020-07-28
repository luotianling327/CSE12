//==========================================================================
// cs12wi20cu                         Homework 7              Tianling Luo
//--------------------------------------------------------------------------
// File: Driver.java
//
// Description: In this assignment, we will write the class implementation
// for UCSDStudent which includes all UCSDStudent methods that will be
// called by the Tree code.
//==========================================================================

import java.io.*;  
      
//=========================================================================
// class UCSDStudent
//
// Description: The UCSDStudent class is defined and it contains the member 
// functions toString, UCSDStudent, getName, equals and isGreaterThan to
// be called by the Tree.java code.
//
// Data Fields:
//     name (String)  - the name of UCSD student
//     studentnum (long)  - the student number of UCSD student
//
// Public functions:
//     UCSDStudent - store the name and student number in name and
//     		     Studentnum with input String and long
//     getName - get the name of the student
//     equals - check if a student name is equal to another one
//     isGreaterThan - to compare the name with base
//     toString - get the name and student number in string
//==========================================================================  
class UCSDStudent extends Base {  
      
        private String name;  // name of student
    private long studentnum;  // student number of student
      
        public UCSDStudent (String nm, long val) {  
            // assign the name
            name = new String (nm); 
            // assign the studentnum
            studentnum = val;  
        }  
  
        public String getName () {  
            // return the name
            return name;  
        }  

        public boolean equals (Object object) {  
            if (this == object)  
                return true;  
  
            if (!(object instanceof UCSDStudent))  
                return false;  
          
            UCSDStudent otherVar = (UCSDStudent) object;  
            // check if the names are the same
            return name.equals (otherVar.getName ());  
        }
        
        public boolean isGreaterThan (Base base) {  
            // return true if it is greater
            return (name.compareTo (base.getName ()) > 0) ? true : false;  
        }    
      
        public String toString () {
            // return the string with name and student name
            return "name:  " + name + "  studentnum:  " + studentnum;  
        }  
}  
      
public class Driver {  
        private static final short NULL = 0;  
  
        public static void main (String [] args) {  
              
            // initialize debug states
            Tree.debugOff();  
    
            // check command line options
            for (int index = 0; index < args.length; ++index) {  
                if (args[index].equals("-x"))  
                    Tree.debugOn();  
            }  
      
      
            // The real start of the code
            SymTab<UCSDStudent> symtab = 
            new SymTab<UCSDStudent>("UCSDStudentTree");  
            String buffer = null;
            char command;  
            long number = 0;  
      
            System.out.println ("Initial Symbol Table:\n" + symtab);  
      
            while (true) {
                command = NULL; // reset command each time in loop  
                System.out.print ("Please enter a command:  " +   
                            "((a)llocate, (i)nsert, (l)ookup, " +  
                            "(r)emove, (w)rite):  ");  
      
                try {  
                    command = MyLib.getchar ();  
                    MyLib.clrbuf (command); // get rid of return  
      
                    switch (command) {  
                    case 'a':  
                            System.out.print  
                            ("Please enter name of new Tree to " +  
                            "allocate:  ");  
                                   
                            buffer = MyLib.getline ();// formatted input  
                            symtab = new SymTab<UCSDStudent>(buffer);  
                            break;  
      
                    case 'i':  
                            System.out.print  
                            ("Please enter UCSD student name to insert:  ");  
      
                            buffer = MyLib.getline ();// formatted input  
      
                            System.out.print  
                                    ("Please enter UCSD student number:  ");  
      
                            number = MyLib.decin ();  
                            MyLib.clrbuf (command); // get rid of return  
     
                            // create student and place in symbol table  
                            symtab.insert(new UCSDStudent (buffer, number));  
                            break;  
   
                    case 'l': {   
                            UCSDStudent found;      // whether found or not  
     
                            System.out.print  
                            ("Please enter UCSD student name to lookup:  ");  
                            buffer = MyLib.getline ();// formatted input  
      
                            UCSDStudent stu = new UCSDStudent (buffer, 0);  
                            found = symtab.lookup (stu);  
                                      
                            if (found != null) {  
                                    System.out.println("Student found!");  
                                    System.out.println(found);  
                            }  
                            else  
                                    System.out.println ("student " + buffer  
                                            + " not there!");  
                            }  
                            break;  
                              
                    case 'r': {   
                            UCSDStudent removed; // data to be removed  
     
                            System.out.print  
                            ("Please enter UCSD student name to remove:  ");  
  
                            buffer = MyLib.getline ();  
   
                            UCSDStudent stu = new UCSDStudent (buffer, 0);  
      
                            removed = symtab.remove(stu);  
      
                            if (removed != null) {  
                                    System.out.println("Student removed!");   
                                    System.out.println(removed);  
                            }  
                            else  
                                    System.out.println ("student " + buffer  
                                            + " not there!");  
                            }  
                            break;  
      
                    case 'w':  
                            System.out.println("The Symbol Table " +  
                            "contains:\n" + symtab);  
                    }  
                    }  
                    catch (EOFException eof) {  
                            break;  
                    }  
            }  
            System.out.println("\nFinal Symbol Table:\n" + symtab);  
    }  
}  
