/* 
 * CS:APP Data Lab 
 * 
 * <Please put your name and userid here>
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
//1
/* 
 * bitXor - x^y using only ~ and & 
 *   Example: bitXor(4, 5) = 1
 *   Legal ops: ~ &
 *   Max ops: 14
 *   Rating: 1
 */
int bitXor(int x, int y) {
  /*
   * a ^ b => (~a & b) | (a & ~b)
   * a | b => ~(~a & ~b)
   */
  return ~(~((~x & y)) & ~((x & ~y)));
}
/* 
 * tmin - return minimum two's complement integer 
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 4
 *   Rating: 1
 */
int tmin(void) {
  /* 0x80000000 */
  return 0x1 << 31;
}
//2
/*
 * isTmax - returns 1 if x is the maximum, two's complement number,
 *     and 0 otherwise 
 *   Legal ops: ! ~ & ^ | +
 *   Max ops: 10
 *   Rating: 2
 */
int isTmax(int x) {
  /*
   * Tmax == ~Tmin
   * x - Tmax = x + (-Tmax) = x + (~Tmax + 1) = x + Tmin + 1
   * But Tmin == 0x1 << 31, which uses the forbidden op <<.
   */
  /* return !(x + ((0x1 << 31) | 0x1)); */

  /*
   * Tmin = Tmax + 1
   * ~Tmin + 1 = Tmin => ~Tmin + 1 - Tmin = 0 => ~Tmin + 1 + ~Tmin + 1 = 0
   * => Tmax + 1 + Tmax + 1 = 0
   * If z + z = 0, then z = 0 or z = Tmin.
   * If z = Tmin, then z - 1 = Tmax, i.e., (Tmax + 1) << 1 = 0
   * If z = 0, then z - 1 = -1 (0xFFFFFFFF), i.e. (0xFFFFFFFF + 1) << 1 = 0
   */
  
  /* printf("%x\n", x + x + 2); */
  /* printf("%x\n", x + 1 + x + 1); */
  /* printf("%x\n", !(x + x + 2)); */
  /* printf("%x\n", !(x + 1 + x + 1)); */

  /* Error, caused by integer overflow, which is an undefined behavior.*/
  /* return (!(x + x + 2)) & (!!(x + 1)); */

  /* You must use x + 1 + x + 1 instead of x + x + 2. */
  return (!(x + 1 + x + 1)) & (!!(x + 1));
}
/* 
 * allOddBits - return 1 if all odd-numbered bits in word set to 1
 *   Examples allOddBits(0xFFFFFFFD) = 0, allOddBits(0xAAAAAAAA) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 2
 */
int allOddBits(int x) {
  /*
   * x & 0xAAAAAAAA == 0xAAAAAAAA
   * y <= 0xAAAAAAAA: y = (0xAA << 24) | (0xAA << 16) | (0xAA << 8) | 0xAA
   */
  int y;
  /* y = (0xAA << 24) | (0xAA << 16) | (0xAA << 8) | 0xAA; */
  // Less ops
  y = (0xAA << 8) | 0xAA;
  y = (y << 16) | y;
  return !((x & y) + ~y + 1);
}
/* 
 * negate - return -x 
 *   Example: negate(1) = -1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 5
 *   Rating: 2
 */
int negate(int x) {
  return ~x + 1;
}
//3
/* 
 * isAsciiDigit - return 1 if 0x30 <= x <= 0x39 (ASCII codes for characters '0' to '9')
 *   Example: isAsciiDigit(0x35) = 1.
 *            isAsciiDigit(0x3a) = 0.
 *            isAsciiDigit(0x05) = 0.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 3
 */
int isAsciiDigit(int x) {
  /*
   * x >= 0x30 => x - 0x30 >= 0 => !((x - 0x30) >> 31)
   * x <= 0x39 => 0x39 - x >= 0 => !((0x39 - x) >> 31)
   */
  return (!((x + ~0x30 + 1) >> 31)) & (!((0x39 + ~x + 1) >> 31));
}
/* 
 * conditional - same as x ? y : z 
 *   Example: conditional(2,4,5) = 4
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 16
 *   Rating: 3
 */
int conditional(int x, int y, int z) {
  /*
   * If x is 0, then y, i.e. y & 0x00000000 | z & 0xFFFFFFFF
   * Else, then z, i.e. y & 0xFFFFFFFF | z & 0x00000000
   * x == 0 => !x == 1, !!x == 0 => !x - 1 == 0,  ~!x + 1 / !!x - 1
   * x != 0 => !x == 0, !!x == 1 => !x - 1 == -1, ~!x + 1 / !!x - 1
   * I think the most interesting part of this function is that
   * when I want to get 0xFFFFFFFF from 0x0, I first think of ~
   * instead of minus op. Actually, if I think 0xFFFFFFFF as -1, I
   * may think about minus op quickly.
   */
  /* return ((y & (!x + ~0x1 + 1)) | (z & (!!x + ~0x1 + 1))); */
  /*
   * If you want to use less ops, you may find that:
   * !!x - 1 == ~!x + 1
   */
  return (y & (!x + ~0x1 + 1)) | (z & (~!x + 1));
}
/* 
 * isLessOrEqual - if x <= y  then return 1, else return 0 
 *   Example: isLessOrEqual(4,5) = 1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 24
 *   Rating: 3
 */
int isLessOrEqual(int x, int y) {
  /*
   * x <= y => y - x >= 0 => !((y - x) >> 31)
   * Fail when overflow!
   */
  /* return (!((y + ~x + 1) >> 31)); */
  /*
   * Consider about sign
   * sign = z >> 31, if z is positive, sign is 0x0, if z is negative, sign is 0xFFFFFFFF
   * so for (x >> 31) ^ (y >> 31), 0 means same signs, and 0xFFFFFFFF means different signs.
   * and then, for ((x >> 31) ^ (y >> 31)) + 1, 1 means same signs, 0 means different signs.
   *
   * We can use variables for less ops.
   */
  int sign_x = x >> 31;
  int sign_y = y >> 31;
  int same_sign = (sign_x ^ sign_y) + 1;
  return (same_sign & !((y + ~x + 1) >> 31)) | (!same_sign & sign_x);

  /*
   * Besides my own solution, I get another solution from internet. Same number of ops,
   but I think it's puzzling.
   */
  /* int sign = sign_x + sign_y; */
  /* return (sign & (sign_x & 1) | ((~sign) & !((y + ~x + 1) >> 31))); */
}
//4
/* 
 * logicalNeg - implement the ! operator, using all of 
 *              the legal operators except !
 *   Examples: logicalNeg(3) = 0, logicalNeg(0) = 1
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 4 
 */
int logicalNeg(int x) {
  /*
   * If x is 0, return 1, else return 0.
   */
  /* x = (x >> 16) | x; */
  /* x = (x >> 8) | x; */
  /* x = (x >> 4) | x; */
  /* x = (x >> 2) | x; */
  /* x = (x >> 1) | x; */
  /* return ~x & 0x1; */

  /*
   * I get a interesting solution from internet, and its dependency is that
   * only when x = 0 or x = 0x80000000, x == -x. And we can distinct them
   * with their MSB.
   */
  return ((~(~x + 1) & ~x) >> 31) & 0x1;
}
/* howManyBits - return the minimum number of bits required to represent x in
 *             two's complement
 *  Examples: howManyBits(12) = 5
 *            howManyBits(298) = 10
 *            howManyBits(-5) = 4
 *            howManyBits(0)  = 1
 *            howManyBits(-1) = 1
 *            howManyBits(0x80000000) = 32
 *  Legal ops: ! ~ & ^ | + << >>
 *  Max ops: 90
 *  Rating: 4
 */
int howManyBits(int x) {
  return 0;
}
//float
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
  return 2;
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
  return 2;
}
/* 
 * float_f2i - Return bit-level equivalent of expression (int) f
 *   for floating point argument f.
 *   Argument is passed as unsigned int, but
 *   it is to be interpreted as the bit-level representation of a
 *   single-precision floating point value.
 *   Anything out of range (including NaN and infinity) should return
 *   0x80000000u.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
int float_f2i(unsigned uf) {
  return 2;
}
