/****************************************************************************

                                                        Tianling Luo
                                                        CSE 12, Winter 2020
                                                        January 12, 2020
                                                        cs12wi20cu
                                Assignment Two

File Name:      hw2.java
Description:    This program reads strings and integers from the user,
                processes them, and prints them back to the user.  Program
                terminates when user enters ^D.  At termination, program
                outputs sizes of various types of java pre defined types.

****************************************************************************/

/**
 * The hw2 class is a direct port of hw2.c to java.
 * As you already know in java when you pass literal strings like
 * <P>
 *   writeline("a literal string\n", stream);
 * <P>
 * in C is considered a char[], but in java it's automatically
 * converted and treated as a String object.  Therefore 
 * the function writeline accepts literal strings and 
 * String types.  The getaline function returns a String type.
 */

import java.io.*;        // System.in and System.out
import java.util.*;      // Stack

class MyLibCharacter {
    private Character character;
    
    public MyLibCharacter (int ch) {
	character = new Character ( (char) ch );
    }
    
    public char charValue () {
	return character.charValue ();
    }
    
    public String toString () {
	return "" + character;
    }
}

public class hw2 {
    private static final int ASCII_ZERO = 48;
    
    private static final int CR = 13;		// Carriage Return
    private static final int MAXLENGTH = 80;	// Max string length
    
    private static final int EOF = -1;		// process End Of File
    
    private static final long COUNT = 16;		// # of hex digits
    
    private static final long DECIMAL = 10;		// to indicate base 10
    private static final long HEX = 16;		// to indicate base 16
    
    private static final char digits[] = 	// for ASCII conversion
	new String("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ").toCharArray();
    
    private static final String DEBUG_GETALINE = 
	"[*DEBUG:  The length of the string just entered is ";
    
    private static final String DIGIT_STRING = "digit ";
    private static final String REENTER_NUMBER ="\nPlease reenter number: ";
    private static final String OUT_OF_RANGE = " out of range!!!\n";
    private static final String CAUSED_OVERFLOW = " caused overflow!!!\n";
    private static final String DEBUG_WRITELINE =
	"\n[*DEBUG:  The length of the string displayed is ";
    
    private static Stack<MyLibCharacter> InStream =
	new Stack<MyLibCharacter>();
    
    private static boolean debug_on = false;
    private static long hexCounter = 0; // counter for the number hex digits
    
    
    /**
        * Takes in a positive number and displays in a given base.
        *
        * @param    Numeric value to be displayed.
        * @param    Base to used to display number.
        * @param    Where to display, likely System.out or System.err.
        */
    private static void baseout (long number, long base, PrintStream stream) {
        // use digit to store each integer in number in character
        char digit; 
	// use index1 to count the total number of digits (total #=index1-1)
        int index1 = 0;
	// use index2 to do iteration in the following part
	int index2 = 0;
	// use array1 to store the numbers backwards
       	char array1[] =new char[(int)COUNT];              
	
	//for the case that number is only 0
       	if (number == 0) {
	    writeline("0", stream);
	    return;
	}
	//for the case that the base is equal to HEX
	if(base==HEX){
	    while (index1!=HEX) {
		digit = digits[(int)(number % base)];
		number = number / base;
		array1[index1] = digit;
		index1++;
	    }
	}
	//for the cases that the base is not HEX
	else{
	    while (number!=0) {
		digit = digits[(int)(number % base)];
		number = number / base;
		array1[index1] = digit;
		index1++;
	    }
	}
	//call writeline to display to stream
	while (index2 < index1){
	    writeline(String.valueOf(array1[index1 - 1 - index2]),stream);
	    index2++;}
    }
    

    /**
        * Removes any characters in the Stystem.in buffer 
	* by repeatedly calling fgetc until a newline character is detected.
        *
        * @param    Character in buffer.
        * @result   Characters in buffer are removed.
        */
    public static void clrbuf (int character){
	// if System.in is already clear
	if (character == '\n') {
	    return;
	}
	// iteratate until the System.in is cleared
	while (character != '\n') {
	    character=fgetc(System.in);
	}
    }


    /**
	* Reads a decimal number from System.in and converts it from
	* a sequence of ASCII characters into a decimal integer.
        *
        * @return    Returns the value in decimal integer or EOF
        */
    public static long decin() {
	int character;      //use it to get character from user
	long sum = 0;     //use it to calculate the sum
	long lastsum = 0;      //use it equal to the last sum to check overflow
	
	while (true) {
	    character = fgetc(System.in);
	    
	    //to check if a new line is added
	    if (character == '\n') {
		clrbuf(character);
		break;
	    }
	    
	    //to check if EOF is added
	    if (character == EOF) {
		return EOF;
	    }
	    
	    //to check if the character is 0-9
	    if (character <= ASCII_ZERO +9 && character >= ASCII_ZERO) {
		lastsum = sum;
		sum = sum * DECIMAL + (character - ASCII_ZERO);
	    }
	    
	    //if the character is not a digit
	    else {
		digiterror(character, OUT_OF_RANGE);
		lastsum = 0;
		sum = 0;
		continue;
	    }
	    
	    //check for overflow
	    if (sum / DECIMAL != lastsum) {
		digiterror(character, CAUSED_OVERFLOW);
		lastsum = 0;
		sum = 0;
	    }
	}
	return sum;
    }
    

    /**
	* Takes in a positive number and displays it in decimal.
        *
        * @param    positive numeric value to be displayed.
        * @param    Where to display, likely System.out or System.err.
        */
    public static void decout (long number, PrintStream stream) {
	baseout(number, DECIMAL,stream);
    }


    /**
        * Takes in a positive number and displays it in decimal.
        *
        * @param    The character that began the problem.
        * @param    The message to display to the user.
        * @result   The message is displayed to the user.
	* The result in progress needs to be set to 0
	* in decin after the call to digiterror.
        */
    public static void digiterror (int character, String message) {

	// handle error
	clrbuf (character);

	// output error message
	writeline (DIGIT_STRING, System.err);
	fputc ( (char)character, System.err);
	writeline (message, System.err);
	
	writeline (REENTER_NUMBER, System.err);
    }


    /**
        * Reads characters from stdin or System.in 
        * and stores them into the message array.
        *
        * @param    Message to be displayed.
        * @param    maxlength of the message.
        * @return   Returns string inputed by user.
        */
    public static long getaline( char message[], int maxlength ) {
	int index = 0;      //use index to calculate the length of message
	int character = 0;      //use it to get characters from user
	int iteration ;     //use it to set elments in message as null
	
	//set elements in message as null
	for (iteration=0;iteration<message.length;iteration++)
	    message[iteration]='\0';

	while (character!='\n') {
	    //to get characters from user
	    character = fgetc(System.in);
	    message[index] = (char)character;

	    //to check if ^D is entered
	    if (character == EOF) {
		return EOF;
	    }
	    
	    //to check if a newline is added
	    if (character == '\n') {
		message[index] = '\0';
		break;
	    }

	    //to check if there are maxlength-1 characters
	    if (index == maxlength - 1) {
		clrbuf(character);
		// set the last character as null
		message[index] = '\0';
		break;
	    }
	    
	    index++;
	}
	//reset the maxlength
	if (index < maxlength)
	    maxlength = index;
	
	//for the debug message to print out when it is needed
	if (debug_on)
	    System.err.print(DEBUG_GETALINE + index + "]\n");
	
	return index;
    }


    /**
        * Takes in a positive number and displays it in hex.
        *
        * @param    A positive numeric value to be displayed in hex.
        * @param    Where to display, likely System.out or System.err.
        */
    public static void hexout (long number, PrintStream stream) {

	// Output "0x" for hexidecimal.
	writeline ("0x", stream);
	baseout (number, HEX, stream);
    }


    /**
        * Returns a character from the input stream.
        *
        * @return  <code>char</code> 
        */
    public static int fgetc(InputStream stream) {

	char ToRet = '\0';
	
        // Check our local input stream first.
        //   If it's empty read from System.in
        if (InStream.isEmpty ()) {

            try {
		// Java likes giving the user the
                // CR character too. Dumb, so just 
                // ignore it and read the next character
                // which should be the '\n'.                  
                ToRet = (char) stream.read ();
                if (ToRet == CR)
                     ToRet = (char) stream.read ();
                                
                // check for EOF
                if ((int) ToRet == 0xFFFF)
                     return EOF;
                }

                // Catch any errors in IO.
                catch (EOFException eof) {

                // Throw EOF back to caller to handle
                return EOF;
                }

                catch (IOException ioe) {

		    writeline ("Unexpected IO Exception caught!\n",
			       System.out);
                    writeline (ioe.toString (), System.out);
                }
	}
	
	// Else just pop it from the InStream.
	else               
	    ToRet = ((MyLibCharacter) InStream.pop ()).charValue ();
	return ToRet;
    }


    /**
        * Displays a single character.
        *
        * @param    Character to display.
        */
    public static void fputc(char CharToDisp, PrintStream stream) {

	// Print a single character.
        stream.print (CharToDisp);   

        // Flush the system.out buffer, now. 
        stream.flush ();
    }


    /**
        * Prints out a newline character.
        * @param    Where to display, likely System.out or System.err.
        *
        */
    public static void newline ( PrintStream stream ) {
	fputc('\n',stream);
    }


    /**
        * Prints out a string.
        *
        * @param    A string to print out.
        * @param    Where to display, likely System.out or System.err.
        * @return     <code>int</code> The length of the string.
        */
    public static long writeline (String message, PrintStream stream) {
        int index = 0;      // to track character
        char mes[]=new String(message).toCharArray();
	while (index<mes.length) {
	    if(mes[index]=='\0')
		break;
	    // output each character
	    fputc(mes[index], stream);
	    // to increment index
	    index++;
	}
	//to print out debug message if needed
	if (debug_on)
	    System.err.print(DEBUG_WRITELINE + index + "]\n");
	
	return index;
    }


    /**
	* Places back a character into the input stream buffer.
	*
	* @param    A character to putback into the input buffer stream.
	*/
    public static void ungetc (int ToPutBack) {
	
	// Push the char back on our local input stream buffer.
	InStream.push (new MyLibCharacter (ToPutBack));
    }


public static void main( String[] args ) {

    char buffer[] = new char[MAXLENGTH];       // to hold string
    
    long number;                  // to hold number entered
    long strlen;                  // length of string
    long base;		      // to hold base entered
    
    // initialize debug states
    debug_on = false;

    // check command line options for debug display
    for (int index = 0; index < args.length; ++index) {
	if (args[index].equals("-x"))
	    debug_on = true;
    } 
    
    // infinite loop until user enters ^D
    while (true) {
	writeline ("\nPlease enter a string:  ", System.out);
	
	strlen = getaline (buffer, MAXLENGTH);
	newline (System.out);
	
	// check for end of input
	if ( EOF == strlen )
	    break;
	
	writeline ("The string is:  ", System.out);
	writeline ( new String(buffer), System.out);
	
	writeline ("\nIts length is ", System.out);
	decout (strlen, System.out);
	newline (System.out);
	
	writeline ("\nPlease enter a decimal number:  ", 
		   System.out);
	if ((number = decin ()) == EOF)
	    break;
	
	writeline ("\nPlease enter a decimal base:  ", 
		   System.out);
	if ((base = decin ()) == EOF)
	    break;
	
	// correct bases that are out of range
	if (base < 2)
	    base = 2;
	else if (base > 36)
	    base = 36;
	
	newline (System.out);
	
	writeline ("Number entered in base ", System.out);
	decout (base, System.out);
	writeline (" is: ", System.out);
	baseout (number, base, System.out);
	
	writeline ("\nAnd in decimal is:  ", System.out);
	decout (number, System.out);
	
	writeline ("\nAnd in hexidecimal is:  ", System.out);
	hexout (number, System.out);
	
	writeline ("\nNumber entered multiplied by 8 is:  ", 
		   System.out);
	decout (number << 3, System.out);
	writeline ("\nAnd in hexidecimal is:  ", System.out);
	hexout (number << 3, System.out);
	
	newline (System.out);
    }
}
}
