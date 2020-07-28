/****************************************************************************

                                                        Tianling Luo
                                                        CSE 12, Winter 2020
                                                        January 12, 2020
                                                        cs12wi20cu
                                Assignment Two

File Name:      hw2.c
Description:    This program reads strings and integers from the user,
                processes them, and prints them back to the user.  Program
                terminates when user enters ^D.  At termination, program
                outputs sizes of various types of C/C++ pre defined types.

****************************************************************************/


/* declare fputc/fgetc */
#include <stdio.h>
#include <getopt.h>

/* define some program-wide constants */
#define ASCII_ZERO '0'
#define COUNT ((long) (sizeof (long) << 1))
#define ERROR -1
#define FALSE 0
#define MAXLENGTH 80
#define OFFSET ('a' - 'A')
#define SPACE ' '
#define TRUE 1

#define DECIMAL 10
#define HEX 16

/* define the keyword "NULL" as 0 */
#ifdef NULL
#undef NULL
#endif
#define NULL 0

/* declarations for functions defined in this file */
void baseout(long number, long base, FILE* stream);
void clrbuf(int);
long decin(void);
void decout(unsigned long, FILE*);
void digiterror(int, long*, const char*);
long getaline(char*, int);
void hexout(unsigned long, FILE*);
void newline(FILE*);
long writeline(const char*, FILE*);

/* array for input checking and for output */
const char digits[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

/* messages */
const char CAUSED_OVERFLOW[] = " caused overflow!!!\n";
const char DIGIT_STRING[] = "digit ";
const char REENTER_NUMBER[] = "\nPlease reenter number: ";
const char OUT_OF_RANGE[] = " out of range!!!\n";

/* debug messages */
const char DEBUG_GETALINE[] =
"[*DEBUG:  The length of the string just entered is ";
const char DEBUG_WRITELINE[] =
"\n[*DEBUG:  The length of the string displayed is ";
static long debug_on = FALSE;


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
  int index2 = 0;
  
  /*for the case that the number is equal to 0 */
  if (number == 0) {
    writeline("0", stream);
  }
  /*for the case that the base is equal to HEX*/
  if (base == HEX) {
    while (index1 != HEX) {
      digit = digits[number % base];
      number = number / base;
      array1[index1] = digit;
      index1++;
    }
  }
  /*for the case that the base is not HEX*/
  else {
    while (number != 0) {
      digit = digits[number % base];
      number = number / base;
      array1[index1] = digit;
      index1++;
    }
  }
  /*convert the array1 and store in array2*/
  while (index2 < index1) {
    array2[index1 - 1 - index2] = array1[index2];
    index2++;
  }
  array2[index1] = '\0';        /* add null to the end of the string */
  writeline(array2, stream);   /* call writeline function */
}


/*--------------------------------------------------------------------------
Function Name:          clrbuf
Purpose:                When called, this function will clear stdin.
Description:            This function checks to see if the incoming
		        parameter is already '\n' indicating that stdin
		        is already clear.  If stdin is already clear, this
		        function does nothing.  Otherwise, this function
		        calls "fgetc" in a loop until stdin is clear.
Input:                  character:  the most recent character received from a
	        	previous call to fgetc.
Result:                 stdin cleared.  Nothing returned.
--------------------------------------------------------------------------*/
void clrbuf(int character) {
  /* if stdin is already clear*/
  if (character == '\n') {
    return;
  }
  /* iteratate until the stdin is cleared */
  while (character != '\n') {
    character=fgetc(stdin);
  }
}


/*--------------------------------------------------------------------------
Function Name:          decin
Purpose:                This function accepts integer input from from the user.
Description:            This function processes user input in a loop that ends
		      	when the user enters either a valid number or EOF.
		  	If EOF is entered, EOF is returned.  Otherwise each
			character entered is checked to verify that it is
			numeric.  Non-numeric input is identified, the user
			is notified, reprompted, and the loop begins again.
			Once the input is verified to be valid, a series
			of multiplication by 10 and addition can take
			place to convert the ASCII characters entered into
			a numeric quantity.
Input:                  None.
Result:                 The number entered or EOF.
--------------------------------------------------------------------------*/
long decin(void) {
  int character;      /*use it to get character from user*/
  long sum = 0;     /*use it to calculate the sum*/
  long lastsum = 0;      /*use it equal to the last sum to check overflow*/
  
  while (character != '\n') {
    character = fgetc(stdin);
    
    /*to check if a new line is added*/
    if (character == '\n') {
      clrbuf(character);
      break;
    }
    
    /*to check if EOF is added*/
    if (character == EOF) {
      return EOF;
    }
    
    /*to check if the character is 0-9*/
    if (character <= ASCII_ZERO +9 && character >= ASCII_ZERO) {
      lastsum = sum;
      sum = sum * DECIMAL + (character - ASCII_ZERO);
    }
    
    /*if the character is not a digit*/
    else {
      digiterror(character, &sum, OUT_OF_RANGE);
      lastsum = 0;
      continue;
    }
    
    /*check for overflow*/
    if (sum / DECIMAL != lastsum) {
      digiterror(character, &sum, CAUSED_OVERFLOW);
      lastsum = 0;
    }
  }
  return sum;
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
Function Name:          digiterror
Purpose:                This function handles erroneous user input.
Description:            This function reinitializes sum, displays and
		        error message to the user, and asks for fresh
			input.
Input:                  character:  The character that began the problem.
		        sum:  A pointer to the sum to reinitialize.
			message:  The message to display to the user.
Result:                 The message is displayed to the user.  sum is 0.
--------------------------------------------------------------------------*/
void digiterror(int character, long* sum, const char* message) {
  
  /* handle error */
  clrbuf(character);
  
  /* reset sum */
  *sum = 0;
  
  /* output error message */
  writeline(DIGIT_STRING, stderr);
  fputc(character, stderr);
  writeline(message, stderr);
  
  writeline(REENTER_NUMBER, stdout);
}


/*--------------------------------------------------------------------------
Function Name:          getaline
Purpose:                This function will read a string from the user.
Description:            This function gets input from the user via
                        calls to fgetc up to some maximum number of
			characters.  Input is terminated when either the
			maximum number of characters are entered, or
			a newline character is detected.  If the user
			enters more characters than the maximum, clrbuf
			is called to remove extra characters which are
			ignored.  Since this is routine accepts input,
			if EOF is detected EOF is passed back to the main
			program.
Input:                  message:  the destination array where input is stored.
                        maxlength:  the maximum number of non-NULL characters
			allowed in the string + 1 for the NULL char.
Result:                 User input is stored in message.
		   	EOF is returned when the user enters ^D.
			Otherwise, the length of the message is returned.
--------------------------------------------------------------------------*/
long getaline(char* message, int maxlength) {
  int index = 0;      /*use index to calculate the length of message*/
  int character = 0;      /*use it to get characters from user*/
  
  while (character!='\n') {
    /*to get characters from user*/
    character = fgetc(stdin);
    message[index] = (char)character;
    
    /*to check if ^D is entered*/
    if (character == EOF) {
      return EOF;
    }
    
    /*to check if a newline is added*/
    if (character == '\n') {
      message[index] = '\0';
      break;
    }
    
    /*to check if there are maxlength-1 characters*/
    if (index == maxlength - 1) {
      clrbuf(character);
      /* set the last character as null*/
      message[index] = '\0';
      break;
    }
    /* to increment index */
    index++;
  }
  /*reset the maxlength*/
  if (index < maxlength)
    maxlength = index;
  
  /*for the debug message to print out when it is needed*/
  if (debug_on)
    fprintf(stderr, "%s%d%s", DEBUG_GETALINE, index,"]");
  return index;
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
Purpose:               To display a newline character
                       ('\n') to the filestream stream.
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
                       return is the lengthof the message
--------------------------------------------------------------------------*/
long writeline(const char* message, FILE* stream) {
  int index = 0;      /* to track character */
  while (message[index] != '\0') {
    /* output each character */
    fputc(message[index], stream);
    /* to increment index */
    index++;
  }
  /*to print out debug message if needed*/
  if (debug_on)
    fprintf(stderr,"%s%d%s",DEBUG_WRITELINE,index,"]\n");
  return index;
}


/*--------------------------------------------------------------------------
Function Name:          main
Description:            This function asks for input and displays output
Purpose:                This program asks the user to enter a string
		       	and a number.  Computations are performed on the
			strings and numbers, and the results are displayed.
Description:            This function asks for input and displays output
			in an infinite loop until EOF is detected.  Once EOF
			is detected, the lengths of the types are displayed.
Input:                  None.
--------------------------------------------------------------------------*/
int main(int argc, char* const* argv) {
  char buffer[MAXLENGTH];      /* to hold string */
  long number;                  /* to hold number entered */
  long strlen;                  /* length of string */
  long base;		      /* to hold base entered */
  
  long array[10];            /* to show user where memory is allocated */
  long* ap = array;	   /* to show user about addresses in memory */
  long** app = &ap;	   /* to show user about addresses in memory */
  long* apx = &array[0];	/* to show user about addresses in memory */
  char option;                 /* the command line option */
  
  /* initialize debug states */
  debug_on = FALSE;
  
  /* check command line options for debug display */
  while ((option = getopt(argc, argv, "x")) != EOF) {
    switch (option) {
    case 'x': debug_on = TRUE; break;
    }
  }
  
  /* infinite loop until user enters ^D */
  while (1) {
    writeline("\nPlease enter a string:  ", stdout);
    strlen = getaline(buffer, MAXLENGTH);
    newline(stdout);
    
    /* check for end of input */
    if (strlen == EOF)
      break;
    
    writeline("The string is:  ", stdout);
    writeline(buffer, stdout);
    
    writeline("\nIts length is ", stdout);
    decout(strlen, stdout);
    newline(stdout);
    
    writeline("\nPlease enter a decimal number:  ", stdout);
    if ((number = decin()) == EOF)
      break;
    
    writeline("\nPlease enter a decimal base:  ", stdout);
    if ((base = decin()) == EOF)
      break;
    
    /* correct bases that are out of range */
    if (base < 2)
      base = 2;
    else if (base > 36)
      base = 36;
    
    newline(stdout);
    
    writeline("Number entered in base ", stdout);
    decout(base, stdout);
    writeline(" is: ", stdout);
    baseout(number, base, stdout);
    
    writeline("\nAnd in decimal is:  ", stdout);
    decout(number, stdout);
    
    writeline("\nAnd in hexidecimal is:  ", stdout);
    hexout(number, stdout);
    
    writeline("\nNumber entered multiplied by 8 is:  ", stdout);
    decout(number << 3, stdout);
    writeline("\nAnd in hexidecimal is:  ", stdout);
    hexout(number << 3, stdout);
    
    newline(stdout);
  }
  
  writeline("\nThe value of ap is:  ", stdout);
  decout((long)ap, stdout);
  writeline("\nAnd in hexidecimal is:  ", stdout);
  hexout((long)ap, stdout);
  newline(stdout);
  
  writeline("The value of app is:  ", stdout);
  decout((long)app, stdout);
  writeline("\nAnd in hexidecimal is:  ", stdout);
  hexout((long)app, stdout);
  newline(stdout);
  
  writeline("The value of apx is:  ", stdout);
  decout((long)apx, stdout);
  writeline("\nAnd in hexidecimal is:  ", stdout);
  hexout((long)apx, stdout);
  newline(stdout);
  
  writeline("The value of ap + 1 is:  ", stdout);
  decout((long)(ap + 1), stdout);
  writeline("\nAnd in hexidecimal is:  ", stdout);
  hexout((long)(ap + 1), stdout);
  newline(stdout);
  
  writeline("The address of array[0] is:  ", stdout);
  decout((long)&array[0], stdout);
  newline(stdout);
  
  writeline("The address of array[1] is:  ", stdout);
  decout((long)&array[1], stdout);
  newline(stdout);
  
  writeline("The size of a float is:  ", stdout);
  decout(sizeof(float), stdout);
  newline(stdout);
  
  writeline("The size of a double is:  ", stdout);
  decout(sizeof(double), stdout);
  newline(stdout);
  
  writeline("The size of a long double is:  ", stdout);
  decout(sizeof(long double), stdout);
  newline(stdout);
  
  writeline("The size of a char is:  ", stdout);
  decout(sizeof(char), stdout);
  newline(stdout);
  
  writeline("The size of an int is:  ", stdout);
  decout(sizeof(int), stdout);
  newline(stdout);
  
  writeline("The size of a short is:  ", stdout);
  decout(sizeof(short), stdout);
  newline(stdout);
  
  writeline("The size of a short int is:  ", stdout);
  decout(sizeof(short int), stdout);
  newline(stdout);
  
  writeline("The size of a long is:  ", stdout);
  decout(sizeof(long), stdout);
  newline(stdout);
  
  writeline("The size of a long int is:  ", stdout);
  decout(sizeof(long int), stdout);
  newline(stdout);
  
  writeline("The size of a long long is:  ", stdout);
  decout(sizeof(long long), stdout);
  newline(stdout);
  
  writeline("The size of a signed is:  ", stdout);
  decout(sizeof(signed), stdout);
  newline(stdout);
  
  writeline("The size of a signed char is:  ", stdout);
  decout(sizeof(signed char), stdout);
  newline(stdout);
  
  writeline("The size of a signed short is:  ", stdout);
  decout(sizeof(signed short), stdout);
  newline(stdout);
  
  writeline("The size of a signed short int is:  ", stdout);
  decout(sizeof(signed short int), stdout);
  newline(stdout);
  
  writeline("The size of a signed int is:  ", stdout);
  decout(sizeof(signed int), stdout);
  newline(stdout);
  
  writeline("The size of a signed long is:  ", stdout);
  decout(sizeof(signed long), stdout);
  newline(stdout);
  
  writeline("The size of a signed long int is:  ", stdout);
  decout(sizeof(signed long int), stdout);
  newline(stdout);
  
  writeline("The size of a signed long long is:  ", stdout);
  decout(sizeof(signed long long), stdout);
  newline(stdout);
  
  writeline("The size of an unsigned is:  ", stdout);
  decout(sizeof(unsigned), stdout);
  newline(stdout);
  
  writeline("The size of an unsigned char is:  ", stdout);
  decout(sizeof(unsigned char), stdout);
  newline(stdout);
  
  writeline("The size of an unsigned short is:  ", stdout);
  decout(sizeof(unsigned short), stdout);
  newline(stdout);
  
  writeline("The size of an unsigned short int is:  ", stdout);
  decout(sizeof(unsigned short int), stdout);
  newline(stdout);
  
  writeline("The size of an unsigned int is:  ", stdout);
  decout(sizeof(unsigned int), stdout);
  newline(stdout);
  
  writeline("The size of an unsigned long is:  ", stdout);
  decout(sizeof(unsigned long), stdout);
  newline(stdout);
  
  writeline("The size of an unsigned long int is:  ", stdout);
  decout(sizeof(unsigned long int), stdout);
  newline(stdout);
  
  writeline("The size of an unsigned long long is:  ", stdout);
  decout(sizeof(unsigned long long), stdout);
  newline(stdout);
  
  writeline("The size of a void pointer is:  ", stdout);
  decout(sizeof(void*), stdout);
  newline(stdout);
  
  writeline("The size of a character pointer is:  ", stdout);
  decout(sizeof(char*), stdout);
  newline(stdout);
  
  writeline("The size of an int pointer is:  ", stdout);
  decout(sizeof(int*), stdout);
  newline(stdout);
  
  writeline("The size of a long pointer is:  ", stdout);
  decout(sizeof(long*), stdout);
  newline(stdout);
  
  writeline("The size of a float pointer is:  ", stdout);
  decout(sizeof(float*), stdout);
  newline(stdout);
  
  writeline("The size of a double pointer is:  ", stdout);
  decout(sizeof(double*), stdout);
  newline(stdout);
  
  writeline("The size of a long double pointer is:  ", stdout);
  decout(sizeof(long double*), stdout);
  newline(stdout);
  
  newline(stdout);
  
  return 0;
}
