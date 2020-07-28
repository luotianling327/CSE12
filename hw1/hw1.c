/****************************************************************************

                                                        Tianling Luo
                                                        CSE 12, winter
                                                        9 Jan 2020
                                                        cs12wi20cu
                                Assignment One

File Name:      hw1.c
Description:    This program tests functions to display output strings and
                numbers.
****************************************************************************/
#include <stdio.h>

#define COUNT ((long) (sizeof (long) << 1 )) /* number of hex digits to
                                                display */
#define DECIMAL 10 /* to indicate base 10 */
#define HEX 16 /* to indicate base 16 */
#define ADD 12 /* To be used in adding values in do while loop in main */

                                                /* Function prototypes */
void baseout(long, long, FILE*);
void decout(unsigned long, FILE*);
void hexout(unsigned long, FILE*);
void newline(FILE*);
long writeline(const char*, FILE*);

/* array used for ASCII conversion */
const char digits[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

/*--------------------------------------------------------------------------
Function Name:         baseout.
Purpose:               takes the long integer number provided by the user,
                       converts it to the base that is passed in,
                       and displays it to the filestream stream
Description:           1. Store the characters in an array in a loop going 
                       forward, print character from array backwards
                       2. Store character in an array from the end of the 
                       backwards, print character from array forwards
                       3. Store NULL first at END of your string (in an array) 
                       and call writeline to print
Input:                 number: the number to be displayed
                       base: the base we use
                       stream: where to display, likely stdout or stderrc
Result:                Number in the given base is displayed.
                       No return value.
--------------------------------------------------------------------------*/
void baseout(long number, long base, FILE* stream) {
    /* use digit to store each integer in number in character */
    char digit;  
    /* use index1 to count the total number of digits (total #=index1-1) */
    int index1 = 0; 
    /* use array1 to store the numbers backwards */            
    char array1[BUFSIZ];
    /* use array2 to store numbers forward*/
    char array2[BUFSIZ];         
    /* use index2 to do itertion */
    int index2=0;
    
    /*for the case that the number is equal to 0 */
    if (number == 0) {
        writeline("0", stream);
    }
    /*for the case that the base is equal to HEX*/
    if(base==HEX){
      while (index1!=HEX) {
        digit = digits[number % base];
        number = number / base;
        array1[index1] = digit;
        index1++;
      }}
    /*for the case that the base is not HEX*/
    else{
      while (number!=0) {
        digit = digits[number % base];
        number = number / base;
        array1[index1] = digit;
        index1++;
      }
}
    /*convert the array1 and store in array2*/
    while (index2 < index1){
        array2[index1 - 1 - index2] = array1[index2];
	index2++;
    }
    array2[index1] = '\0';        /* add null to the end of the string */
    writeline(array2, stream);   /* call writeline function */
}


/*--------------------------------------------------------------------------
Function Name:         decout
Purpose:               Prints a number in base 10 to the parameter FIle stream
Description:           Goal is achieved via delegating to the baseout function
Input:                 number:  the number to display
                       stream:  where to display, likely stdout or stderrc
Result:                Number in base 16 is displayed.
                       No return value.
--------------------------------------------------------------------------*/
void decout(unsigned long number, FILE* stream) {
    /*call the baseout function in decout */
    baseout(number, DECIMAL, stream);
}


/*--------------------------------------------------------------------------
Function Name:         hexout
Purpose:               Prints a number in base 16 to the parameter FILE stream
Description:           Goal is achieved via delegating to the baseout function
Input:                 number:  the number to display
stream:  where to display, likely stdout or stderrc
Result:                Number in base 16 is displayed.
                       No return value.
--------------------------------------------------------------------------*/
void hexout(unsigned long number, FILE* stream) {

    /* Output "0x" for hexidecimal. */
    writeline("0x", stream);
    baseout(number, HEX, stream);
}


/*--------------------------------------------------------------------------
Function Name:         newline.
Purpose:               
To display a newline character ('\n') to the filestream stream.
Description:           Use fput to add '\n'.
Input:                 stream: where to display, likely stdout or stderr.
Result:                No return value.
                       There should be a newline added to stream.
--------------------------------------------------------------------------*/
void newline(FILE* stream) {
    /*use fputc to add '\n'*/
    fputc('\n', stream);
}


/*--------------------------------------------------------------------------
Function Name:         writeline
Purpose:               
Displays the string message to the user via the filestream stream.
Description:           use a loop and fputc to display each element in an array
Input:                 message: the string to display
stream: where to dislay, likely stdout or stderr.
Result:                write the message to the file stream
the return value is the length 
--------------------------------------------------------------------------*/
long writeline(const char* message, FILE* stream) {
    int index = 0;      /* to track character */
    while (message[index] != '\0') {
        /* output each character */
        fputc(message[index], stream);
        /* to increment index */
        index++;
    }
    return index-1;
}


int main(int argc, char* const* argv) {

    int element = 0;
    int count = 0;

    do {
        element += ADD;
        count++;

    } while (count < 3);

    writeline("def", stdout);
    writeline("abc", stderr);
    newline(stderr);
    newline(stdout);
    decout(123, stdout);
    newline(stdout);
    decout(0, stdout);
    newline(stdout);
    hexout(0xFEEDDAD, stdout);
    newline(stdout);
    return 0;
}
