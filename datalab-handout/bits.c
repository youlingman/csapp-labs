/* 
 * CS:APP Data Lab 
 * 
 * <Please put your name and userid here>
 * cyl
 *
 * bits.c - Source file with your solutions to the Lab.
 *          This is the file you will hand in to your instructor.
 *
 * WARNING: Do not include the <stdio.h> header; it confuses the dlc
 * compiler. You can still use printf for debugging without including
 * <stdio.h>, although you might get a compiler warning. In general,
 * it's not good practice to ignore compiler warnings, but in this
 * case it's OK.  
 */

#if 0
/*
 * Instructions to Students:
 *
 * STEP 1: Read the following instructions carefully.
 */

You will provide your solution to the Data Lab by
editing the collection of functions in this source file.

INTEGER CODING RULES:
 
  Replace the "return" statement in each function with one
  or more lines of C code that implements the function. Your code 
  must conform to the following style:
 
  int Funct(arg1, arg2, ...) {
      /* brief description of how your implementation works */
      int var1 = Expr1;
      ...
      int varM = ExprM;

      varJ = ExprJ;
      ...
      varN = ExprN;
      return ExprR;
  }

  Each "Expr" is an expression using ONLY the following:
  1. Integer constants 0 through 255 (0xFF), inclusive. You are
      not allowed to use big constants such as 0xffffffff.
  2. Function arguments and local variables (no global variables).
  3. Unary integer operations ! ~
  4. Binary integer operations & ^ | + << >>
    
  Some of the problems restrict the set of allowed operators even further.
  Each "Expr" may consist of multiple operators. You are not restricted to
  one operator per line.

  You are expressly forbidden to:
  1. Use any control constructs such as if, do, while, for, switch, etc.
  2. Define or use any macros.
  3. Define any additional functions in this file.
  4. Call any functions.
  5. Use any other operations, such as &&, ||, -, or ?:
  6. Use any form of casting.
  7. Use any data type other than int.  This implies that you
     cannot use arrays, structs, or unions.

 
  You may assume that your machine:
  1. Uses 2s complement, 32-bit representations of integers.
  2. Performs right shifts arithmetically.
  3. Has unpredictable behavior when shifting an integer by more
     than the word size.

EXAMPLES OF ACCEPTABLE CODING STYLE:
  /*
   * pow2plus1 - returns 2^x + 1, where 0 <= x <= 31
   */
  int pow2plus1(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     return (1 << x) + 1;
  }

  /*
   * pow2plus4 - returns 2^x + 4, where 0 <= x <= 31
   */
  int pow2plus4(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     int result = (1 << x);
     result += 4;
     return result;
  }

FLOATING POINT CODING RULES

For the problems that require you to implent floating-point operations,
the coding rules are less strict.  You are allowed to use looping and
conditional control.  You are allowed to use both ints and unsigneds.
You can use arbitrary integer and unsigned constants.

You are expressly forbidden to:
  1. Define or use any macros.
  2. Define any additional functions in this file.
  3. Call any functions.
  4. Use any form of casting.
  5. Use any data type other than int or unsigned.  This means that you
     cannot use arrays, structs, or unions.
  6. Use any floating point data types, operations, or constants.


NOTES:
  1. Use the dlc (data lab checker) compiler (described in the handout) to 
     check the legality of your solutions.
  2. Each function has a maximum number of operators (! ~ & ^ | + << >>)
     that you are allowed to use for your implementation of the function. 
     The max operator count is checked by dlc. Note that '=' is not 
     counted; you may use as many of these as you want without penalty.
  3. Use the btest test harness to check your functions for correctness.
  4. Use the BDD checker to formally verify your functions
  5. The maximum number of ops for each function is given in the
     header comment for each function. If there are any inconsistencies 
     between the maximum ops in the writeup and in this file, consider
     this file the authoritative source.

/*
 * STEP 2: Modify the following functions according the coding rules.
 * 
 *   IMPORTANT. TO AVOID GRADING SURPRISES:
 *   1. Use the dlc compiler to check that your solutions conform
 *      to the coding rules.
 *   2. Use the BDD checker to formally verify that your solutions produce 
 *      the correct answers.
 */


#endif
/* 
 * bitAnd - x&y using only ~ and | 
 *   Example: bitAnd(6, 5) = 4
 *   Legal ops: ~ |
 *   Max ops: 8
 *   Rating: 1
 */
int bitAnd(int x, int y) {
//  return 2;
	return ~((~x) | (~y));
}
/* 
 * getByte - Extract byte n from word x
 *   Bytes numbered from 0 (LSB) to 3 (MSB)
 *   Examples: getByte(0x12345678,1) = 0x56
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 6
 *   Rating: 2
 */
int getByte(int x, int n) {
//  return 2;
	return (x >> (n << 3)) & 0xFF;
}
/* 
 * logicalShift - shift x to the right by n, using a logical shift
 *   Can assume that 0 <= n <= 31
 *   Examples: logicalShift(0x87654321,4) = 0x08765432
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 3 
 */
int logicalShift(int x, int n) {
  //return 2;
	int mask = ~(0x1 << 31 >> n << 1);
	return (x >> n) & mask;
}
/*
 * bitCount - returns count of number of 1's in word
 *   Examples: bitCount(5) = 2, bitCount(7) = 3
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 40
 *   Rating: 4
 */
int bitCount(int x) {
  //return 2;
	//from hamming weight's wiki page
	//http://en.wikipedia.org/wiki/Hamming_weight#Efficient_implementation
	int m;
	m = 0x55;
	m = m | (m << 8);
	m = m | (m << 16);
	//m = 0x55555555
	x = ((x >> 1) & m) + (x & m);//giving 1s in each 2-bit piece of x
	m = 0x33;
	m = m | (m << 8);
	m = m | (m << 16);
	// m = 0x33333333
	x = ((x >> 2) & m) + (x & m);//giving 1s in each 4-bit piece of x
	m = 0x0f;
	m = m | (m << 8);
	m = m | (m << 16);
	// m = 0x0f0f0f0f
	x = ((x >> 4) & m) + (x & m);//giving 1s in each 8-bit piece of x
	m = 0xff;
	m = m | (m << 16);
	// m = 0x00ff00ff
	x = ((x >> 8) & m) + (x & m);//giving 1s in each 16-bit piece of x
	m = 0xff;
	m = m | (m << 8);
	// m = 0000ffff
	return (((x >> 16) & m) + (x & m));
}
/* 
 * bang - Compute !x without using !
 *   Examples: bang(3) = 0, bang(0) = 1
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 4 
 */
int bang(int x) {
   //return 2;
	return ((x | (~x + 1)) >> 31) + 1;
}
/* 
 * tmin - return minimum two's complement integer 
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 4
 *   Rating: 1
 */
int tmin(void) {
  //return 2;
	return 0x1 << 31;
}
/* 
 * fitsBits - return 1 if x can be represented as an 
 *  n-bit, two's complement integer.
 *   1 <= n <= 32
 *   Examples: fitsBits(5,3) = 0, fitsBits(-4,3) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 2
 */
int fitsBits(int x, int n) {
//  return 2;
	int sign = x >> 31 & 0x1;
	return !((x >> (n + (~0x0))) + sign);
}
/* 
 * divpwr2 - Compute x/(2^n), for 0 <= n <= 30
 *  Round toward zero
 *   Examples: divpwr2(15,1) = 7, divpwr2(-33,4) = -2
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 2
 */
int divpwr2(int x, int n) {
    //return 2;
	//negative number need carry after div if the fraction part is not 0x0
	int all_sign = x >> 31;
	int drop_mask = ((0x1 << n) + (~0x0)) & all_sign;
	return (x + drop_mask) >> n;
}
/* 
 * negate - return -x 
 *   Example: negate(1) = -1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 5
 *   Rating: 2
 */
int negate(int x) {
  //return 2;
	return (~x)+0x1;
}
/* 
 * isPositive - return 1 if x > 0, return 0 otherwise 
 *   Example: isPositive(-1) = 0.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 8
 *   Rating: 3
 */
int isPositive(int x) {
  //return 2;
	return (!(x >> 31)) & (!!x);
}
/* 
 * isLessOrEqual - if x <= y  then return 1, else return 0 
 *   Example: isLessOrEqual(4,5) = 1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 24
 *   Rating: 3
 */
int isLessOrEqual(int x, int y) {
  //return 2;
	int sign_x = x >> 31 & 0x1;
	int sign_y = y >> 31 & 0x1;
	int not_same_sign = sign_x ^ sign_y;
	//int x_minus_y_minus_1 = x + ((~y) + 0x1) + ~(0x0);
	int x_minus_y_minus_1 = x + (~y);
	int sign_x_minus_y_minus_1 = x_minus_y_minus_1 >> 31 & 0x1;
	return (sign_x & not_same_sign) | (sign_x_minus_y_minus_1 & (!not_same_sign));
}
/*
 * ilog2 - return floor(log base 2 of x), where x > 0
 *   Example: ilog2(16) = 4
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 90
 *   Rating: 4
 */
int ilog2(int x) {
//  return 2;
	int m;

	//make the hisiest sign bit fulfill all lower bits
	x = x >> 1;
	x = x | (x >> 1);
	x = x | (x >> 2);
	x = x | (x >> 4);
	x = x | (x >> 8);
	x = x | (x >> 16);

	//count the 1s in x
	m = 0x55;
	m = m | (m << 8);
	m = m | (m << 16);
	//m = 0x55555555
	x = ((x >> 1) & m) + (x & m);//giving 1s in each 2-bit piece of x
	m = 0x33;
	m = m | (m << 8);
	m = m | (m << 16);
	// m = 0x33333333
	x = ((x >> 2) & m) + (x & m);//giving 1s in each 4-bit piece of x
	m = 0x0f;
	m = m | (m << 8);
	m = m | (m << 16);
	// m = 0x0f0f0f0f
	x = ((x >> 4) & m) + (x & m);//giving 1s in each 8-bit piece of x
	m = 0xff;
	m = m | (m << 16);
	// m = 0x00ff00ff
	x = ((x >> 8) & m) + (x & m);//giving 1s in each 16-bit piece of x
	m = 0xff;
	m = m | (m << 8);
	// m = 0000ffff
	return (((x >> 16) & m) + (x & m));
}
/* 
 * float_neg - Return bit-level equivalent of expression -f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representations of
 *   single-precision floating point values.
 *   When argument is NaN, return argument.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 10
 *   Rating: 2
 */
unsigned float_neg(unsigned uf) {
	int sign_mask = 0x80000000;
	int exponent = uf & 0x7F800000;
	int fraction = uf & 0x007FFFFF;
	if (exponent == 0x7F800000 && fraction != 0x0) sign_mask = 0x0;
	return uf ^ sign_mask;
 //return 2;
}
/* 
 * float_i2f - Return bit-level equivalent of expression (float) x
 *   Result is returned as unsigned int, but
 *   it is to be interpreted as the bit-level representation of a
 *   single-precision floating point values.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
unsigned float_i2f(int x) {
  //return 2;
	int sign, exponent, fraction;

	if (x == 0x0) return 0x0;
	if (x == 0x80000000) return 0xCF000000;
	sign = x & 0x80000000;
	if (sign == 0x80000000) x = (~x) + 1;
	exponent = 0x0;
	// cut the left continuou 0s in x
	while(!(x & 0x80000000)) {
		exponent ++;
		x = x << 1;
	}
	// get the 23-bit fraction part
	fraction = (x & 0x7FFFFF00) >> 8;
	// do the carry work in fraction
	fraction = fraction + (((x & 0xFF) > 0x80) || ((x & 0x180) == 0x180));
	if (fraction & 0x00800000) {
		exponent --;
		fraction = (fraction & (~0x00800000)) >> 1;
	}
	exponent = (158 - exponent) << 23;
	return (sign + exponent + fraction);
}
/* 
 * float_twice - Return bit-level equivalent of expression 2*f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representation of
 *   single-precision floating point values.
 *   When argument is NaN, return argument
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
unsigned float_twice(unsigned uf) {
  //return 2;
	//http://en.wikipedia.org/wiki/Single_precision#Exponent_encoding
	int sign = uf & 0x80000000;
	int exponent = uf & 0x7F800000;
	//int fraction = uf & 0x007FFFFF;
	if(exponent == 0x7F800000) return uf;//NaN or infinity
	else if(exponent == 0x0) return (uf << 1) | sign;//zero or denormal numbers
	else return (uf + 0x00800000);//normalized value
}
