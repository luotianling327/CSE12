/****************************************************************************

                                                        Tianling Luo
                                                        CSE 12, winter
                                                        9 Jan 2020
                                                        cs12wi20cu
                                Assignment One

File Name:      hw1.java
Description:    This program tests functions to display output strings and
                numbers.
****************************************************************************/


/**
 * The hw1 class is a direct port of hw1.c to java.
 * As you already know in java when you pass literal strings like
 * <P>
 *   writeline("a literal string\n", stream);
 * <P>
 * in C is considered a char[], but in java it's automatically
 * converted and treated as a String object.  Therefore 
 * the function writeline accepts literal strings and 
 * String types.
 */

import java.io.*;      // System.out

public class hw1 {
	private static final long COUNT = 16;	// number of hex digits to display
	private static final long DECIMAL = 10;	// to indicate base 10
	private static final long HEX = 16;		// to indicate base 16
	private static final int ADD = 12;	// to add to the var in the main loop

	private static final char digits[] = 	// used for ASCII conversion
		new String("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ").toCharArray();
	private static long hexCounter = 0; // counter for the number hex digits


	/**
	* Takes in a positive number and displays in a given base.
        *
        * @param    Numeric value to be displayed.
        * @param    Base to used to display number.
        * @param    Where to display, likely System.out or System.err.
        */
    private static void baseout (long number, long base, PrintStream stream) {
        /* use digit to store each integer in number in character */
        char digit; 
	/* use index1 to count the total number of digits (total #=index1-1) */
        int index1 = 0;
	/* use index2 to do iteration in the following part */
	int index2=0;
	/* use array1 to store the numbers backwards */
       	char array1[] =new char[(int)COUNT];              
	
	/*for the case that number is only 0*/
       	if (number == 0) {
	    writeline("0", stream);
	    return;
		}
	/*for the case that the base is equal to HEX*/
	if(base==HEX){
	    while (index1!=HEX) {
		digit = digits[(int)(number % base)];
		number = number / base;
		array1[index1] = digit;
		index1++;
	    }}
	/*for the cases that the base is not HEX*/
	else{
	    while (number!=0) {
		digit = digits[(int)(number % base)];
		number = number / base;
		array1[index1] = digit;
		index1++;
	    }}
	/*call writeline to display to stream*/
	while (index2 < index1){
	    writeline(String.valueOf(array1[index1 - 1 - index2]),stream);
	    index2++;}
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
        * Displays a single character.
        *
        * @param    Character to display.
        * @param    Where to display, likely System.out or System.err.
        */
        public static void fputc(char CharToDisp, PrintStream stream) {

	    /* Print a single character.*/
	    stream.print (CharToDisp);   

	    /* Flush the system.out buffer, now. */
	    stream.flush ();
        }


        /**
        * Takes in a positive number and displays it in hex.
        *
        * @param    A positive numeric value to be displayed in hex.
        * @param    Where to display, likely System.out or System.err.
        */
        public static void hexout (long number, PrintStream stream) {

	    /* Output "0x" for hexidecimal.*/
	    writeline ("0x", stream);
	    baseout (number, HEX, stream);
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
            int index = 0;      /* to track character */
            char mes[]=new String(message).toCharArray();
	    while (index<mes.length) {
		/* output each character */
		fputc(mes[index], stream);
		/* to increment index */
		index++;
	    }

	    return index-1;
        }


	public static void main( String[] args ) {

	    int element = 0;
	    int count = 0;
	    while (count < 3){
		
		element += ADD;
		count++;
	    }
	    writeline ("abc", System.err);
	    newline(System.err);
	    writeline ("def", System.out);
	    newline(System.out);
	    decout (123, System.out);
	    newline(System.out);
	    decout (0, System.out);
	    newline(System.out);
	    hexout (0xFEEDDAD, System.out);
	    newline(System.out);
	}
}
