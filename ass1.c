/* Program to perform multi-digit integer arithmetic.

   Skeleton program written by Alistair Moffat, ammoffat@unimelb.edu.au,
   August 2023, with the intention that it be modified by students
   to add functionality, as required by the assignment specification.
   All included code is (c) Copyright University of Melbourne, 2023

   Student Authorship Declaration:

   (1) I certify that except for the code provided in the initial skeleton
   file, the  program contained in this submission is completely my own
   individual work, except where explicitly noted by further comments that
   provide details otherwise.  I understand that work that has been developed
   by another student, or by me in collaboration with other students, or by
   non-students as a result of request, solicitation, or payment, may not be
   submitted for assessment in this subject.  I understand that submitting for
   assessment work developed by or in collaboration with other students or
   non-students constitutes Academic Misconduct, and may be penalized by mark
   deductions, or by other penalties determined via the University of
   Melbourne Academic Honesty Policy, as described at
   https://academicintegrity.unimelb.edu.au.

   (2) I also certify that I have not provided a copy of this work in either
   softcopy or hardcopy or any other form to any other student, and nor will I
   do so until after the marks are released. I understand that providing my
   work to other students, regardless of my intention or any undertakings made
   to me by that other student, is also Academic Misconduct.

   (3) I further understand that providing a copy of the assignment
   specification to any form of code authoring or assignment tutoring service,
   or drawing the attention of others to such services and code that may have
   been made available via such a service, may be regarded as Student General
   Misconduct (interfering with the teaching activities of the University
   and/or inciting others to commit Academic Misconduct).  I understand that
   an allegation of Student General Misconduct may arise regardless of whether
   or not I personally make use of such solutions or sought benefit from such
   actions.

   Signed by: Phan Minh Quan Nguyen 1450700
   Dated:     5/9/2023

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include <assert.h>
#include <unistd.h>
#include <math.h>

/* All necessary #defines provided as part of the initial skeleton */

#define INTSIZE	500	/* max number of digits per integer value */
#define LINELEN	999	/* maximum length of any input line */
#define NVARS	26	/* number of different variables */

#define CH_A     'a'    /* character 'a', first variable name */

#define ERROR	(-1)	/* error return value from some functions */
#define INT_OVERFLOW (-2)  /* error when integers overflows*/
#define PROMPT	"> "	/* the prompt string for interactive input */

#define MAX_DIGIT_EXPONENT 4 /* as the max exponent is 1661, so the digit of the
								expoent max is 4 */
								
#define PASS 1 /* return when it passes the overflow_check for longint_t*/
#define FAIL 0 /* return when it doesn't pass the overflow checl*/

#define PRINT	'?'	/* the print operator */
#define ASSIGN	'='	/* the assignment operator */
#define PLUS	'+'	/* the addition operator */
#define MULT	'*'	/* the multiplication operator */
#define POWR	'^'	/* the power-of operator */
#define DIVS	'/'	/* the division operator */
#define ALLOPS  "?=+*^/"

#define CH_ZERO  '0'    /* character zero */
#define CH_ONE   '1'    /* character one */

#define CH_COM   ','    /* character ',' */
#define PUT_COMMAS 3    /* interval between commas in output values */

#define INT_ZERO 0	/* integer 0 */
#define INT_ONE 1  /* integer 1 */
#define INT_TEN  10	/* integer 10 */

/* Placeholder typedef for skeleton code
*/
typedef int longint_t[INTSIZE + INT_ONE];
#define LONG_ZERO {1,0} /* represent 0 for longint_t type */
#define LONG_ONE  {1,1} /* represent 1 for longint_t type */


/****************************************************************/

/* A "magic" additional function needing explicit declaration */
int fileno(FILE *);

/* Skeleton program function prototypes */

void print_prompt(void);
void print_tadaa();
void print_error(char *message);
int  read_line(char *line, int maxlen);
void process_line(longint_t vars[], char *line);
int  get_second_value(longint_t vars[], char *rhsarg,
	longint_t *second_value);
int  to_varnum(char ident);
void do_print(int varnum, longint_t *var);
void do_assign(longint_t *var1, longint_t *var2);
void do_plus(longint_t *var1, longint_t *var2);
void carry_checking(longint_t *var, int carry);
void zero_vars(longint_t vars[]);
void parse_num(char *rhs, longint_t *second_value);
int pass_overflow_test(int length);
int max(int num1, int num2);
int min(int num1, int num2);
void do_digit_mult(longint_t *var1, longint_t *temp_result,
		int digit, int factor_order);
void do_mult(longint_t *var1, longint_t *var2);
void do_power(longint_t *var1, longint_t *var2);
void overflow_detects();


/****************************************************************/

/* Main program controls all the action
*/
int
main(int argc, char *argv[]) {
	char line[LINELEN+1] = {0};
	longint_t vars[NVARS];

	zero_vars(vars);
	print_prompt();
	while (read_line(line, LINELEN)) {
		if (strlen(line) > 0) {
			/* non empty line, so process it */
			process_line(vars, line);
		}
		print_prompt();
	}

	print_tadaa();
	return 0;
}

/****************************************************************/

/* Prints the prompt indicating ready for input, but only if it
can be confirmed that the input is coming from a terminal.
Plus, output might be going to a file, that's why the prompt,
if required, is written to stderr and not stdout
*/
void
print_prompt(void) {
	if (isatty(fileno(stdin))) {
		fprintf(stderr, "> ");
		fflush(stderr);
	}
}

void
print_tadaa() {
	/* all done, so pack up bat and ball and head home,
		getting the exact final lines right is a bit tedious,
		because input might be coming from a file and output
		might be going to a file */
	if (isatty(fileno(stdin)) && isatty(fileno(stdout))) {
		printf("\n");
	}
	printf("ta daa!!!\n");
	if (isatty(fileno(stdin)) && !isatty(fileno(stdout))) {
		fprintf(stderr, "\n");
	}
}

void
print_error(char *message) {
	/* need to write an error message to the right place(s)
	*/
	if (isatty(fileno(stdin)) || isatty(fileno(stdout))) {
		fprintf(stderr, "%s\n", message);
		fflush(stderr);
	}
	if (!isatty(fileno(stdout))) {
		printf("%s\n", message);
	}
}

/****************************************************************/

/* Reads a line of input into the array passed as argument,
returns false if there is no input available.
All whitespace characters are removed on the way through.
*/
int
read_line(char *line, int maxlen) {
	int i=0, c;
	while (((c=getchar())!=EOF) && (c!='\n')) {
		if (i<maxlen && !isspace(c)) {
			line[i++] = c;
		}
	}
	line[i] = '\0';
	/* then, if the input is coming from a file or the output
		is going to a file, it is helpful to echo the input line
		and record what the command was */
	if (!isatty(fileno(stdin)) || !isatty(fileno(stdout))) {
		printf("%s%s\n", PROMPT, line);
	}
	return ((i>0) || (c!=EOF));
}

/****************************************************************/

/* Process a command by parsing the input line into parts
*/
void
process_line(longint_t vars[], char *line) {
	int varnum, optype, status;
	longint_t second_value = LONG_ZERO;

	/* determine the LHS variable, it
		must be first character in compacted line
	*/
	varnum = to_varnum(line[0]);
	if (varnum==ERROR) {
		print_error("invalid LHS variable");
		return;
	}

	/* more testing for validity 
	*/
	if (strlen(line)<2) {
		print_error("no operator supplied");
		return;
	}

	/* determine the operation to be performed, it
		must be second character of compacted line
	*/
	optype = line[1];
	if (strchr(ALLOPS, optype) == NULL) {
		print_error("unknown operator");
		return;
	}

	/* determine the RHS argument (if one is required),
		it must start in the third character of compacted line
	*/
	if (optype != PRINT) {
		if (strlen(line)<3) {
			print_error("no RHS supplied");
			return;
		}
		status = get_second_value(vars, line+2, &second_value); 
		if (status==ERROR) {
			print_error("RHS argument is invalid");
			return;
		} 
		/* Detect if the input is overflow and terminate the program*/
		else if (status==INT_OVERFLOW) {
			overflow_detects();
		}
	}

	/* finally, do the actual operation
	*/
	if (optype == PRINT) {
		do_print(varnum, vars+varnum);
	} else if (optype == ASSIGN) {
		do_assign(vars+varnum, &second_value);
	} else if (optype == PLUS) {
		do_plus(vars+varnum, &second_value);
	} else if (optype == MULT) {
		do_mult(vars+varnum, &second_value);
	} else if (optype == POWR) {
		do_power(vars+varnum, &second_value);
	} else {
		print_error("operation not available yet");
		return;
	}
	return;
}

/****************************************************************/

/* Convert a character variable identifier to a variable number
*/
int
to_varnum(char ident) {
	int varnum;
	varnum = ident - CH_A;
	if (0<=varnum && varnum<NVARS) {
		return varnum;
	} else {
		return ERROR;
	}
}

/****************************************************************/

/* Process the input line to extract the RHS argument, which
should start at the pointer that is passed
*/
int
get_second_value(longint_t vars[], char *rhsarg,
		longint_t *second_value) {
	char *p;
	int varnum2;
	if (isdigit(*rhsarg)) {
		/* first character is a digit, so RHS is a number
			now check the rest of RHS for validity */
		for (p=rhsarg+1; *p; p++) {
			if (!isdigit(*p)) {
				/* nope, found an illegal character */
				return ERROR;
			}
		}
		/* Check if the overflow of the input before converting*/
		int input_length = strlen(rhsarg);
		if (!pass_overflow_test(input_length)) {
			return INT_OVERFLOW;
		}

		/* nothing wrong, ok to convert */
		parse_num(rhsarg, second_value);	
		return !ERROR;
	} else {
		/* argument is not a number, so should be a variable */
		varnum2 = to_varnum(*rhsarg);
		if (varnum2==ERROR || strlen(rhsarg)!=1) {
			/* nope, not a variable either */
			return ERROR;
		}
		/* and finally, get that variable's value */
		do_assign(second_value, vars+varnum2);
		return !ERROR;
	}
	return ERROR;
}

/* Set the vars array to all zero values
*/
void
zero_vars(longint_t vars[]) {
	int i;
	longint_t zero = LONG_ZERO;
	for (i=0; i<NVARS; i++) {
		do_assign(vars+i, &zero);
	}
	return;
}

/* Create an internal-format number out of a string
*/
void
parse_num(char *rhs, longint_t *second_value) {
	/* Count the digit and assign to the first index */ 
	int length = strlen(rhs);
	(*second_value)[0] = length;

	/* Convert the string and put that into reverse order */ 
	for (int i = length; i > 0; i--) {
		(*second_value)[i] = (int)rhs[length-i] - (int)CH_ZERO;
	}
}


/****************************************************************/

/* Print out a longint value
*/
void
do_print(int varnum, longint_t *var) {
	printf("register %c: ", varnum+CH_A);
	int length = (*var)[0];
	/* Check leading zero, and find the real length of the number 
		after eliminate leading 0s */
	while (((*var)[length]) == 0) {
		length--;
	}
	/* The variable haven't initialized, the value of the variable is 0 */
	if (length == 0) {
		printf("%d", INT_ZERO);
	}

	/* Loop backward to print the number in order */
	for (int i = length;  i > INT_ZERO; i--) {
		/* Put the comma in the number */
		if (i % PUT_COMMAS == INT_ZERO && i < length) {
			printf("%c", CH_COM);
		}
		printf("%d", (*var)[i]);
	}
	printf("\n");
}

/****************************************************************/

/* Assign a longint value var2 to var1
*/
void
do_assign(longint_t *var1, longint_t *var2) {
	int length_var2 = (*var2)[0];
	for (int i = 0; i <= length_var2; i++) {
		(*var1)[i]  = (*var2)[i];
		}
}

/****************************************************************/

/* STAGE 1: ADDITION 
*/
void
do_plus(longint_t *var1, longint_t *var2) {
	int length_var1 = (*var1)[0], length_var2 = (*var2)[0];
	int unit_sum, carry = 0;
	int longer_length, shorter_length;
	/* Find the number with less/more digits between var1 and var2 */
	shorter_length = min(length_var1, length_var2);
	longer_length = max(length_var1, length_var2);
	
	/* Set the digit of the result to the number with more digits */
	(*var1)[0] = longer_length;

	for (int i = 1; i <= longer_length; i++) {
		
		if (i <= shorter_length) {
			/* Compute the unit_sum when both var1 and var2 include the postition */
			unit_sum = (*var1)[i] + (*var2)[i] + carry;
		} else {
			/* In case there's one number with more digit than orther */
			if (length_var1 > length_var2) {
			unit_sum = (*var1)[i] + carry;
			} else {
				unit_sum = (*var2)[i] + carry;
			}
		}

		/* When the unit_sum has 1 digit, store it directly */
		if (unit_sum < INT_TEN) {
			(*var1)[i] = unit_sum;
			carry = 0;
		} else {
			/* When unit_sum is 10 or more, store that excess of
			   10 to carry to bring to the next position */
			(*var1)[i] = unit_sum - INT_TEN;
			carry = 1; 
		}
	}

	/* New digit will be add to array if there is carry after the final addition */
	if (carry) {
		/* Increment the length of the answer as we add a new digit */
		(*var1)[0]++;
		/* Assign the carry to the proper position and chheck the integer oveflow */
		carry_checking(var1, carry);
	}
}

/****************************************************************/

/* STAGE 2: MULTIPLICATION 
*/
void 
do_mult(longint_t *var1, longint_t *var2) {
	longint_t temp_result = LONG_ZERO;
	longint_t result = LONG_ZERO;
	int factor_order, curr_digit;
	int length_var2 = (*var2)[0], length_result;

	/* Multiply each digit of var2 with var1 */ 
	for (int i = 1; i <= length_var2; i++) {
		/* Define the current digit and its in var2 */
		factor_order = i;
		curr_digit = (*var2)[i];

		do_digit_mult(var1, &temp_result, curr_digit, factor_order);
		/* Upload the temporary result by adding a new factor */ 
		do_plus(&result, &temp_result);
		
		/* Check the current result as the sum of factor can cause overflow */
		length_result = result[0];
		if (!pass_overflow_test(length_result)) {
			overflow_detects();
		}
	}
	do_assign(var1, &result);
}

/****************************************************************/
/* Helper function for stage 1 and 2, function will add a new digit 
	when there's carry at the last arithmetic and check for integer
	overflow with the new number 
*/
void
carry_checking(longint_t *var, int carry) {
	int length = (*var)[0];
	if (pass_overflow_test(length)) {
		/* Assign the new digit into the most-right of the longint_t */
		(*var)[length] = carry;
	} else {
		overflow_detects();
	}
}

/****************************************************************/

/* Helper function for stage 2, this function performs multiplication 
	between longint_t and a single digit, and set it to temporary 
	result with the correct order of that digit.
*/
void
do_digit_mult(longint_t *var1, longint_t *temp_result, 
			int digit, int factor_order) {
	int factor, remainder, carry = 0;
	int length_var1 = (*var1)[0];
	/* Set the number of digit of the factor, with respect to order */ 
	(*temp_result)[0] = length_var1;

	/* Set 0s to the result and increment the length according to the order */
	for (int i = 1; i< factor_order; i++) {
		(*temp_result)[0]++;
		(*temp_result)[i] = INT_ZERO;
	}

	/* Do digit multiplication to every digit in var1*/
	for (int i = 1; i <= length_var1; i++) {
		factor = digit * (*var1)[i] + carry;
		remainder = factor % INT_TEN;
		/* Store the remainder to the result array to the proper position*/
		(*temp_result)[i + factor_order - INT_ONE] = remainder;
		carry = factor / INT_TEN;
	}
	if (carry) {
		/* Increment the length of the temp_result as there's carry after 
			the last operation */
		(*temp_result)[0]++;
		/* Assign the carry to the proper position and check the integer oveflow 
		for the result */
		carry_checking(temp_result, carry);
	}
}

/****************************************************************/

/*STAGE 3a: Power
*/
void
do_power(longint_t *var1, longint_t *var2) {
	int length_var1 = (*var1)[0], length_var2 = (*var2)[0];
	int first_digit_var1 = (*var1)[1], first_digit_var2 = (*var2)[1];
	longint_t base, long_one = LONG_ONE;
	int exp, power_10;
	
	do_assign(&base, var1);

	/* When the base = 1 and base 0, the answer is the same as the base */
	if (length_var1 == 1 && (first_digit_var1 == 1 || first_digit_var1 == 0)) {
		return;
	} else if (length_var2 == 1 && first_digit_var2 == 0) {
		/* When the exponential part is 0 and the base's not 0 or 1, the answer's 1 */
		do_assign(var1, &long_one);
		return;
	}

	/* Before conver var2 to integer, check if var2 is convertiable */
	if (length_var2 > MAX_DIGIT_EXPONENT) {
		overflow_detects();
	}

	exp = 0;
	power_10 = 1;
	/* Ok, var2 is valud, the convert var2 into integer by add up each digit
		with proper 0s behind depend on the position */
	for (int i = 1; i<= length_var2; i++) {
		exp += (*var2)[i] * power_10;
		power_10 *= 10;
	}

	/* As var1 is the base, (var1) ^ (var2) is the multiplication 
		of var1 to itself var2 times */
	while (exp > INT_ONE) {
		do_mult(var1, &base);
		exp--;
	}
}


/****************************************************************/
/* OTHER HELPER FUNCTION*/

/* Print out the integer detect message and exit the program
*/
void 
overflow_detects() {
	print_error("Integer overflows detects");
	exit(EXIT_FAILURE);
}

/****************************************************************/

/* Return a bigger number from two numbers
*/
int 
max(int num1, int num2) {
	if (num1 >= num2) {
		return num1;
	}
	return num2;
}

/****************************************************************/

/* Return a smaller number from two number
*/
int 
min(int num1, int num2) {
	if (num1 >= num2) {
		return num2;
	}
	return num1;
}

/****************************************************************/

/*Check the overflow INTSIZE of the variable 
*/
int 
pass_overflow_test(int length) {
	if (length > INTSIZE) {
		return FAIL;
	}
	return PASS;
}

/****************************************************************/

/*algorithms are fun*/