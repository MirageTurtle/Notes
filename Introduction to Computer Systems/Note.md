# Introduction to Computer Systems

Textbook: [Computer Systems: A Programmer's Perspective, Third Edition](http://csapp.cs.cmu.edu), and David suggests that a really good strategy for studying and preparing for this course would be to read each chapter *three times*.

Class web page: [http://www.cs.cmu.edu/~213](http://www.cs.cmu.edu/~213)

I want to note all the thing when I take the classes, but I couldn't. So I just note something I think that's very interesting or very important. And I will enrich them when I read the textbook. And, I really recommand you all to take the classes!

## Lecture 1: Course Overviewn

> Create Time: 2023.07.11  Update Time: 2023.07.11

### Course Theme

Abstraction is good but don't forget reality. Coding is that you just put some text into a box, and the purpose of this course is to give you enough understanding of what the box is doing when it executes your code, and through that to help you become better at what you're trying to do.

#### Great Reality #1

Ints are not integers, and floats are not reals.

```lldb
(lldb) print 40000 * 40000
(int) $0 = 1600000000
(lldb) print 50000 * 50000
(int) $1 = -1794967296
(lldb) print 300 * 400 * 500
(int) $2 = 60000000
(lldb) print 300 * 400 * 500 * 600
(int) $3 = 1640261632
(lldb) print (1e20 + -1e20) + 3.14
(double) $4 = 3.1400000000000001
(lldb) print 1e20 + (-1e20 + 3.14)
(double) $5 = 0
```

#### Great Reality #2

You've got to know assembly

As Randal says, you'll never write programs in assembly, because compilers are much better and more patient than you are, but, **understanding assembly** is key to machine-level execution model. And this courses looks at the language of Intel 64-bit processors.

#### Great Reality #3

Memory Matters.

Random access memory is an unphysical abstraction, and memory is not unbounded.

C and C++ do not provide any memory protection. It is a particular feature of these languages. :)

```C
typedef struct
{
    int a[2];
    double d;
} struct_t;

double fun(int i)
{
    volatile struct_t s;
    s.d = 3.14
    s.a[i] = 1073741824; /* Possibly out of bounds */
    return s.d;
}
```

When you execute this code and set `i` from 0 to 6, you can get something interesting.

#### Great Reality #4

There's more to performance than asymptotic complexity.

You need to understand what the system does, what makes it run well and what makes it run poorly.

```C
void copyij(int src[2048][2048], int dst[2048][2048])
{
    int i, j;
	for (i = 0; i < 2048; i++)
	    for (j = 0; j < 2048; j++)
		    dst[i][j] = src[i][j]
}
```

```C
void copyij(int src[2048][2048], int dst[2048][2048])
{
    int i, j;
	for (j = 0; j < 2048; j++)
	    for (i = 0; i < 2048; i++)
		    dst[i][j] = src[i][j]
}
```

What you'll find if you run these two codes on a typical system is that the first one is much faster than the second one.

#### Great Reality #5

Computers do more than execute programs.

They need to get data in and out, and they communicate with each other over networks.

### Course Perspective

A programmer's perspective, and it means let you, as a person who sits in front of a computer screen typeing code, know about the machine you're typing code for in order to be effective at doing it, as spposed to somebody who some day goes to be designing and building the actual machine itself.

## Lecture 2: Bits, Bytes and Integers 1

>  Create Time: 2023.07.12  Update Time: 2023.07.13

> I'll going to pass through the part of base 2 number representatoin.

### Bits and Bytes

- Everything is bits, and each bit is 0 or 1. This is because of the electronic implementation.
- We usually group collections of 4bits at a time and then represent that in base 16 which is known as hexademical representatoin.
- And a byte means 8 bits.
- When they say it's a 64-bit machine, what they really mean is that the addresses are 64-bit values or 8-byte values.

#### Boolean algebra

- `A&B = 1` when both A=1 and B=1
- `A|B = 1` when either A=1 and B=1
- `~A = 1` when A=0
- `A^B=1` when either A=1 or B=1, but not both

#### Shift operations

- Left shift `x << y`: shift bit-vector x left y positions
  - Throw away extra bits on left
  - Fill with 0's on right
- Right shift `x >> y`: shift bit-vector x right y positions
  - Throw away extra bits on right
  - Logical shift: fill with 0's on left
  - Arithmetic shift: replicate most significant bit on left
- Undefined behavior
  - shift amount < 0 or >= word size
	- On most machine, you'll get whatever x is.

### Numer Representation

#### Encoding integers

Here's one interesting idea about how to compute signed number with two's complement. When I took my C programming course in my freshman year, I got when I got a bit pattern, such as `1010`, and the sign bit is `1`, so I needed to ignore the sign bit, and bitwise negated the number, then I got `101` which ignored the sign bit, and it means 5 based 10, finally, I could increment on 5 and add the negative sign and I got the correct number `-6`. But, here, Randal tells, **for signed number, the sign bit is negative. Only this thing, and there's no other differences between unsigned numbers and signed number.** For bit pattern `1010`, you can use $1\times2^3+0\times2^2+1\times2^1+0\times2^0=10$ for unsigned number, and $1\times(-2^3)+0\times2^2+1\times2^1+0\times2^0=-6$. You can find that my old method is equivalent to this one, but the latter is so exquisite!

#### Ranges of a bit pattern with w bits

- Unsigned values: $UMin=0(00..00)$ and $UMax=2^w-1(11..11)$
- Two's complement values: $TMin=-2^{w-1}(100..00)$ and $TMax=2^{w-1}-1(011..11)$

##### Observation

- $|TMax|=|TMin|-1$
- $|UMax|=2\times|TMax|+1$

#### Mapping between signed & unsigned

You can find the mapping rule easily: when the highest bit is 0, they represent the same number, and when the highest bit is 1, their difference is $2^w$. And the mapping is unique.

#### Signed & unsigned in C

- Constants by default are considered to be signed integers.
- If there is a mix of unsigned and signed in single expression, *signed values implicitly cast to unsigned*.

### Expanding and Truncating

#### Sign extension

Given $w$-bit signed integer $x$, and now you want to convert it to $w+k$-bit integer with same value. You should just make $k$ copies of the sign bit.

#### Expanding

- Unsigned: zeros added
- Signed: sign extension

#### Truncating

- Unsigned/Signed: bits are truncated
- Result reinterpreted:
  - Unsigned: mod operation
  - Signed: similar to mod operation

## Lecture 3: Bits, Bytes and Integers 2

>  Create Time: 2023.07.14  Update Time: 2023.07.14

### Addition and Subtraction

Add the two $w$-bit numbers and just truncate $w$ bits if need. You can try to understand it by something mod operation. And you can just do subtraction by doing addition with a negative number in two's complement. So **the additions in unsigned and two's complement have identical bit-level behavior**, which is the reason two's complement is the most popular pattern for signed numbers.

You need to notice the **overflow**, which happens when the true result is $w+1$ bits and trucated.

### Multiplication

As the same as addition, you just truncate $w$ bits if you need. And so does the overflow.

### Power-of-2 Multiply and Divide with Shift

- $u << k$ gives $u * 2^k$

- $u >> k$ gives $\lfloor u / 2^k \rfloor$
  - For signed number, use arithmetic shift. In C there's no fixed requirement for the type of shift, but on most machines, it will be arithmetic shift.

### Negate

Standard way: complement and increment. (The way my teacher teached, but I think core of the two's complement should not be this. This is just a way to compute the nagative number.)
