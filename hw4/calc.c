/**************************************************************************** 
 
                                                        Tianling Luo 
                                                        CSE12, winter 
                                                        Jan 27, 2020
                                                        cs12wi20cu
                                Assignment Four 
 
File Name:      calc.c
Description:    This program will simulate a top-of-the-line calculator.
This program will accept mathematical expressions in "infix" notation
and convert it to "postfix" notation and then evaluate the expression 
with positive integer inputs.
****************************************************************************/

#include <ctype.h>
#include <stdio.h>
#include "calc.h"
#include "mylib.h"
#include "stack.h"

#define CHARACTER(word) ((char) ((word) & 0x000000FF))
#define INDEX(word) (((word) & 0x0000FF00) >> 8)
#define PRIORITY(word) ((word) & 0x0000FE00)
#define SIGN_BIT (1L << ((sizeof (long) << 3) - 1))
#define BYTE 8

static char operators[] = "()+-*/^ !";


static long add (long, long);
static long divide (long, long);
static long exponent (long, long);
static long fact (long, long);
static long mult (long, long);
static long setupword (int);
static long sub (long, long);

static long (*functions[]) (long, long) =
    { NULL, NULL, add, sub, mult, divide, exponent, NULL, fact };

/*--------------------------------------------------------------------------
Function Name:         eval.
Purpose:               Evaluate mathematical expressions from "postfix"
                       notation.
Description:           stack2 holds positive integers and operators, 
		       stack1 only holds signed integers.
Input:                 stack1: pointers to Stacks containing "postfix"
                       expressions to evaluate.
Result:                return the calculated result.
--------------------------------------------------------------------------*/
long eval(Stack* stack1) {
    Stack* stack2;
    /* use item to store the things popped or pushed */
    long item;
    /* use operand1 and operand2 to store the operands */
    long operand1, operand2;
    /* use result to store the result of computation */
    long result;
    /* initialize operand2 to be 0 if not used */
    operand2=0;
    /* construct a local stack stack2 */
    stack2 = new_Stack(CALCSTACKSIZE);

    /* reverse stack1 onto stack2 */
    while (!isempty_Stack(stack1)) {
        pop(stack1, &item);
        push(stack2, item);
    }

    /* while stack2 is not empty */
    while (!isempty_Stack(stack2)) {
        /* pop numbers from stack2 */
        pop(stack2, &item);
        /* if it's a number */
        if (item >= 0) {
            push(stack1, item);
        }
        /* if it's not a number */
        else {
            pop(stack1, &operand1);
            /* if the operator is not fact */
            if (INDEX(item) != BYTE)
                pop(stack1, &operand2);
            result = functions[INDEX(item)](operand1, operand2);
            push(stack1, result);
        }
    }
    /* delete the stack2 */
    delete_Stack(&stack2);
    /* empty stack1 */
    pop(stack1,&result);
    return result;
}

/*--------------------------------------------------------------------------
Function Name:         intopost.
Purpose:               Utilizing two Stacks, convert "infix" mathematical
		       expressions entered by the user into their "postfix"
		       equivalents. 
Description:           Process each character of the input in turn,
		       and then pop from stack2, push to stack1.
Input:                 take a pointer to a stack (stack1) as a parameter.
Result:                either EOF when ^D is entered or a non-zero value
		       indicating that the function succeeded.
--------------------------------------------------------------------------*/
long intopost (Stack * stack1) {

     /* use character to store each character */
     int character;
     /* use number to store decimal number into stack1 */
     long number;
     /* use item to store the poped value */
     long item;
     /* construct a local stack stack2 */
     Stack* stack2=new_Stack(CALCSTACKSIZE);
     character=fgetc(stdin);

     while (character!='\n'){
	
	/* if character is EOF, return EOF */
	if (character==EOF){
		delete_Stack(&stack2);
		return EOF;
       	}

	/* if character is blank, just ignore it */
	if (character==' '){
		character=fgetc(stdin);
		continue;
	}

	/* if character is a digit, then continue until a non-digit
	 * store the number in stack1 */
	if (isdigit(character)){
		ungetc(character,stdin);
		number=decin();
		push(stack1,number);
	}

	/* else if character is '('
	 * then push it tp stack2 */
	else if (character=='('){
		push(stack2,setupword(character));
	}

	/* else if character is ')' then repeatedly pop stack2,
	 * pushing all symbols popped from stack2 onto stack1
	 * until '(' is encountered */
	else if (character==')'){
		if (!isempty_Stack(stack2))
			pop(stack2,&item);
		while (CHARACTER(item)!='('){	
			push(stack1,item);
			if (!isempty_Stack(stack2))
				pop(stack2,&item);
		}
	}
	
	/* else, repeatedly push to stack1 what is popped from stack2
	 * until stack2 is empty or stack2's top symbol has a lower
	 * priority than the character entered. */
	else{
		while(!isempty_Stack(stack2)){
			/* to get stack2's top symbol if it's not empty */
			top(stack2,&item);
			/* to check priority of stack2's top symbol
			 * and the character entered */
			if (PRIORITY(setupword(character))>PRIORITY(item)){
				break;
			}
			pop(stack2,&item);
			push(stack1,item);
		}
		
		/* then, push the character onto stack2 */
		push(stack2,setupword(character));
     }
	character=fgetc(stdin);	     
}

     /* pop anything remaining on stack2,
      * pushing all symbols popped from stack2 to stack1. */
     while (!isempty_Stack(stack2)){
	     pop(stack2,&item);
	     push(stack1,item);
     }
     /* delete stack2 */
     delete_Stack(&stack2);
     return 1;
}

static long add (long augend, long addend) {
    return augend + addend;
}

static long divide (long divisor, long dividend) {
    return dividend / divisor;
}

/*--------------------------------------------------------------------------
Function Name:         exponent.
Purpose:               Raising base to the power exponent.
Description:           Use iteration to multiply base power times
Input:                 base: base of the exponent.
                       power: power of the exponent.
Result:                return the base to the power of power.
--------------------------------------------------------------------------*/
static long exponent(long power, long base) {
    /* iteration is used to do iteration power times */
    int iteration;
    /* use answer to store the final answer, initialize as 1 */
    long answer = 1;
    for (iteration = 0; iteration < power; iteration++) {
        answer *= base;
    }
    return answer;
}

/*--------------------------------------------------------------------------
Function Name:         fact.
Purpose:               Calculating xxx factorial.
Description:           use iteration to multiply from 1 to xxx.
Input:                 xxx: positive integer whose factorial to be calculated.
                       ignored: any valid integer.
Result:                return the factorial of xxx.
--------------------------------------------------------------------------*/
static long fact(long xxx, long ignored) {
    /* iteration is used to do iteration xxx times */
    int iteration;
    /* use answer to store the final answer, initialize as 1 */
    long answer = 1;
    for (iteration = 0; iteration < xxx; iteration++) {
        answer *= (iteration + 1);
    }
    return answer;
}

static long mult (long factorx, long factory) {
    return factorx * factory;
}

/*--------------------------------------------------------------------------
Function Name:         setupword.
Purpose:               Constructor funtion for longs representing operators
		       to be stored on your Stacks.
Description:           Let the returning value contain the sign-bit; index;
		       and the priority of the operator and the ASCII code.
Input:                 Takes the character which is an operator.
Result:                Return the value for operators to be stored on stack.
--------------------------------------------------------------------------*/
static long setupword (int character) {
     /* to track porgress in array */
     long index=0;
     while (operators[index]){
	     if(character==operators[index])
		     break;
	     index++;
     }
     /* let the return value contains sign-bit, index,
	priority of the operator and the ASCII code */
     return SIGN_BIT | (index<<BYTE) | character;
}

static long sub (long subtrahend, long minuend) {
    return minuend - subtrahend;
}

