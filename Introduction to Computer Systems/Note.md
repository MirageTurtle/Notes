# Introduction to Computer Systems

Textbook: [Computer Systems: A Programmer's Perspective, Third Edition](http://csapp.cs.cmu.edu), and David suggests that a really good strategy for studying and preparing for this course would be to read each chapter *three times*.

Class web page: [http://www.cs.cmu.edu/~213](http://www.cs.cmu.edu/~213)

I want to note all the thing when I take the classes, but I couldn't. So I just note something I think that's very interesting or very important. And I will enrich them when I read the textbook. And, I really recommend you all to take the classes!

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

>  Create Time: 2023.07.14  Update Time: 2023.07.15

### Arithmetic Operations

#### Addition and Subtraction

Add the two $w$-bit numbers and just truncate $w$ bits if need. You can try to understand it by something mod operation. And you can just do subtraction by doing addition with a negative number in two's complement. So **the additions in unsigned and two's complement have identical bit-level behavior**, which is the reason two's complement is the most popular pattern for signed numbers.

You need to notice the **overflow**, which happens when the true result is $w+1$ bits and trucated.

#### Multiplication

As the same as addition, you just truncate $w$ bits if you need. And so does the overflow.

#### Power-of-2 Multiply and Divide with Shift

- $u << k$ gives $u * 2^k$

- $u >> k$ gives $\lfloor u / 2^k \rfloor$
  - For signed number, use arithmetic shift. In C there's no fixed requirement for the type of shift, but on most machines, it will be arithmetic shift.

#### Negate

Standard way: complement and increment. (The way my teacher teached, but I think core of the two's complement should not be this. This is just a way to compute the nagative number.)

### Byte-Oriented Memory Organization

In your mind, **the memory is just a big array of bytes** which is numbered from 0 up to some maximum number. In fact what happens is that the operation system only allows certain regions within that memory to be referenced, and if you try to access the other regions it will signal an error what they call *segmentation fault*.

#### Machine Words

There's no really fixed idea of what a word size is. But roughly speaking, the word size is the nominal size of integer-valued data and of addresses. When we talk about a 64-bit machine, what we mean is that it regularly and routinely manipulate 64-bit values and arithmetic operations and it has a pointers or the values of addresses are 64-bit. *Actually, the word size of a program is determined by the combination of the hardware and the compiler.*

#### Addresses

We group the memory into blocks of words, and we usually assume that the address of the word is the lowest value address in it.

#### Byte Ordering

How are the bytes within a multi-byte word ordered in memory? There's little endian and big endian. For little endian, the first byte in the word is the least significant byte and big endian does the opposite.

- Big Endian: Sun, PPC Mac, Internet
- Little Endian: x86, ARM processors running Android, iOS and Windows

You can code to print byte representation of data:

```C
typedef unsigned char *pointer;

void show_bytes(pointer start, size_t len)
{
    size_t i;
    for(i = 0; i < len; i++)
        printf("%p\t0x%.2x\n", start+i, start[i]);
    printf("\n");
    return;
}
```

#### Representing Strings

Different with numbers, byte ordering of strings in C is not an issue. This is because strings is represented by array of characters in C. But there're some tricky things for strings in C: each character encoded in ASCII format and string should be null-terminated.

### Integers C puzzles

> I recommend you try to figure them out following the class which can help you determine how well you're learning.

Why `x & (x-1) != 0`?

### Extented Reading

- [A weird question for Tmax(0x7fffffff), why (!x) == x?](https://stackoverflow.com/questions/62534011/a-weird-question-for-tmax0x7fffffff-why-x-x)

- [Why is unsigned integer overflow defined behavior but signed integer overflow isn't?](https://stackoverflow.com/questions/18195715/why-is-unsigned-integer-overflow-defined-behavior-but-signed-integer-overflow-is)


## Lecture 4: Floating Point

> Create Time: 2023.07.16  Update Time: 2023.07.16

For fractional binary number representation, it's no different for bit pattern representation for integer numbers. But there's some limitation:

- It can only exactly represent numbers of the form $x/2^k$, because other rational numbers have repeating bit representations
- Just one setting of binary point within the $w$ bits, which means you must do some trade-off between number range and number precision. This is where the *floating* comes.

### Floating Point Representation (IEEE standard)

- Numerical Form: $(-1)^s * M * 2^E$
  - Sign bit $s$ determines whether number is negative or positive
  - Significand $M$ normally a fractional value in range [1.0, 2.0].
  - Exponent $E$ weights value by power of two.
- Encoding: `s` + `exp` + `frac`
  - MSB(most significant bit) S is sign bit $s$
  - exp field encodes $E$ but is not equal to $E$
  - frac field encodes $M$ but is not equal to $M$
- Precision options
  - Single precision(32 bits): `s`(1 bit) + `exp`(8 bits) + `frac`(23 bits)
  - Double precision(64 bits): `s`(1 bit) + `exp`(11 bits) + `frac`(52 bits)
  - Extended precision(80 bits, Intel only, not-standard): `s`(1 bit) + `exp`(15 bits) + `frac`(63 or 64 bits)

#### Types

- Normalized values
  - `exp` != 00..00 and `exp` != 11..11
  - Exponent coded as a biased value: `E` = `Exp` - `Bias`
    - `Exp` is the unsigned value of `exp` field
    - `Bias` = $2^{k-1} - 1$, where $k$ is number of exponent bits
  - Significand coded with implied leading 1: `M` = 1.xx..xx
    - xx..xx: bits of `frac` field
    - minimum when `frac`=00..00
    - maximum when `frac`=11..11

- Denormalized values (for some so small numbers closed to zero)
  - `exp` = 00..00
  - Exponent value: `E` = 1 - `Bias` instead of `E` = 0 - `Bias`
  - Significand coded with implied leading 0: `M` = 0.xx..xx
- Special values
  - `exp` = 11..11
  - `frac` = 00..00: representing infinity which comes when operations overflow.
  - `frac` != 00..00: representing Not-a-Number(NaN) which comes when no numeric value can be determined.

#### Visualization and Example

Here's the visualization of the floating number ranges and types.

![visualization](./figures/floating_visualization.png)

Here's an example with `s`(1 bit) + `exp`(4 bits) + `frac`(3 bits).

![Example](./figures/floating_example.png)

#### Distribution

You can find the distribution gets denser toward zero, like this:

![distribution fig1](./figures/floating_distribution1.png)

![distribution fig2](./figures/floating_distribution2.png)

You can find that, when exp field increments, the spacing between two numbers will double. This is the reason the distribution gets denser toward zero.

And at the same time, you can find the spacings of denormalized numbers and the set of the smallest normalized numbers are the same. This can be explained by the rules of denormalized number and normalized number. This is also a manifest for the nice smooth transition.

#### Operations

##### Basic idea

- First compute exact result
- Make it fit into desired precision
  - Possibly overflow if exponent too large
  - Possibly round to fit into `frac`

##### Rounding

- Towards zero
- Round down (towards negative infinity)
- Round up (towards positive infinity)
- **Nearest even (default)**

##### Multiplication

$(-1)^s * M * 2^E = (-1)^{s_1} * M_1 * 2^{E_1} \times (-1)^{s_2} * M_2 * 2^{E-2}$

- $s = s_1 + s_2 = s_1 \oplus s_2$
- $M = M_1 \times M_2$
- $E = E_1 + E_2$

You need fixing:

- If $M \geq 2$, by shifting $M$ right and incrementing $E$
- If $E$ out of range, overflow to infinity.
- Round M to fit `frac` precision.

Compared to Commutative Ring, Floating Point multiplication is not associative and not distributed over addition.

##### Addition

You need to align the binary points by shifting before adding. Actually, the smaller `exp` is aligned to the bigger one. There may be overflow if the difference between two `exp`s is too big.

Compared to Abelian Group, Floating Point addition is not associative because of overflow.

#### Casting in C

- From double/float to int:
  - Just truncating fractional part
  - Like rounding toward zero
  - Not defined when out of range or NaN: generally sets to TMin
- From int to double:
  - Exact conversion, as long as the word size (normally 32) is less than 53 (the size of frac field of double floating point)
- From int to float
  - Will round according to rounding mode.

## Lecture 5: Machine-Level Programming I: Basics

> Create Time: 2023.07.27  Update Time: 2023.07.28

> Pass the stories of Intel, x86, IA64, CISC, RISC, and ARM in my note, but it's also interesting to listen.

### C, assembly, machine code

#### Definitions

- Architecture (also ISA, Instruction Set Architecture): the parts of a processor design that one needs to understand or write assembly/machine code.
- Microarchitecture: Implementation of the architecture.
- Machine Code: the byte-level programs that a processor executes.
- Assembly Code: A text representation of machine code.

#### Machine/Assembly Code View

- PC: Program counter, actually address of next instruction, which is called "RIP" (`%rip`) for x86-64.
- Register file: heavily used program data
- Condition code: store status information about most recent arithmetic or logical operation for conditional branching.
- Memory: Byte addressable array for code and user data.
- NO cache from Code View, there's no instruction for cache. It's not visible to programmer.

#### Turning C into object code

Command: `gcc –Og p1.c p2.c -o p`, where `-O` means optimization, `g` means for debug (more readable), `-o` means output file.

After *preprocessing*, which is something simple like string replacement, there's still three steps to turn C into object code:

- Compile: from C programs to asm programs
- Assemble: from asm programs to object programs
- Link: from object programs and static libraries to executable program.

There's some dynamic libraries, which is linked when executing the program.

![Turning C into object code](./figures/machine_basic_compiling.png)

##### Compiling to Assembly

Command: `gcc –Og -S sum.c`, where `-S` means stop after this step (compiling to assembly)

##### Assembly Characteristics

> I'm not clear about this part, and I'll update when I totally understand them.

- Data Types
- Operations

##### Disassembling Object Code

Command: `objdump -d executable_file`

### Assembly Basics: Registers, operands, move

#### Registers

![Registers](./figures/machine_basic_registers.png)

As this image shows, there's a special register, `%rsp`, which is stack pointer.

#### Moving Data

Command: `move source, dest`

Operand Types: Immediate (Constant integer data, e.g., `$0x400`), Register (one of 16 integer registers, e.g., `%rax`) and Memory (8 consecutive bytes of memory at address given by register, e.g., `(%rax)`).

Operand Combinations: `movq Imm, Reg`, `movq Imm, Mem`, `movq Reg, Reg`, `movq Reg Mem`, `move Mem Reg`. An Immediate is no meaning for a dest, and we can not do memory-memory transfer with a single instruction.

#### Single Memory Addressing Mode

- Normal: `(R)` is standing for `Mem[Reg[R]]`.
- Displacement: `D(R)` is standing for `Mem[Reg[R] + D]`.

#### Complete Memory Addressing Mode

- Most Genral Form: `D(Rb, Ri, S)` is standing for `Mem[Reg[Rb] + S * Reg[Ri] + D]`
  - D: Constant "displacement" 1, 2, or 4 bytes, omitting when D is 0.
  - Rb: Base register, any of 16 integer registers.
  - Ri: Index register, any of left 15 integer rigisters except for `%rsp`.
  - S: Scale, 1, 2, 4, or 8, omitting when S is 1.

### Arithmetic & logical operations

#### Address Computation Instruction

Command: `leaq source, dest`, where `source` is address mode expression, and `dest` is set to address denoted by expression.

#### Some Arithmetic Operations

##### Two Operand Instructions

![Two Operand Instructions](./figures/machine_basic_2op.png)

##### One Operand Instructions

![One Operand Instructions](./figures/machine_basic_1op.png)

## Lecture 6: Machine-Level Programming II: Control

> Create Time: 2023.07.28  Update Time: 2023.08.01

![Information about currently executing program](./figures/machine_control_information.png)

### Condition codes

They are some signle bit registers.

- CF: Carry flag for unsigned, 1 if result carries out from the most significant bit (unsigned overflow).
- ZF: Zero flag, 1 if the result is 0.
- SF: Sign flag for signed, 1 if result is less than 0 as signed.
- OF: Overflow flag for signed, if signed overflow (including positive overflow and negative overflow).

#### Implicitly set

- Arithmetic operations: It's something like side effect.
- Compare: `cmpq src2, src1`, like computing `src1 - src2`
- Test: `testq src2, src1`, like computing `src1 & src2`, and it's useful when `src1` and `src2` are the same.

#### Explicitly set

**SetX intructions**: set low-order byte of destination to 0 or 1 based on combinations of condition codes.

| setX    | Condition         | Description               |
| ------- | ----------------- | ------------------------- |
| `sete`  | ZF                | Equal/Zero                |
| `setne` | ~ZF               | Not Equal/Not Zero        |
| `sets`  | SF                | Negative                  |
| `setns` | ~SF               | Nonnegative               |
| `setg`  | ~ (SF ^ OF) & ~ZF | Greater (Signed)          |
| `setge` | ~ (SF ^ OF)       | Greater or Equal (Signed) |
| `setl`  | (SF ^ OF)         | Less (Signed)             |
| `setle` | (SF ^ OF) \| ZF   | Less or Equal (Signed)    |
| `seta`  | ~CF & ~ZF         | Above (Unsigned)          |
| `setb`  | CF                | Below (Unsigned)          |

### Conditional branches

Jumping with **JX instructions** (Jump to different part of code depending on condition codes.)

| jX   | Condition         | Description               |
| ---- | ----------------- | ------------------------- |
| jmp  | 1                 | Unconditional             |
| je   | ZF                | Equal / Zero              |
| jne  | ~ZF               | Not Equal / Not Zero      |
| js   | SF                | Negative                  |
| jns  | ~SF               | Nonnegative               |
| jg   | ~ (SF ^ OF) & ~ZF | Greater (Signed)          |
| jge  | ~ (SF ^ OF)       | Greater or Equal (Signed) |
| jl   | (SF ^ OF)         | Less (Signed)             |
| jle  | (SF ^ OF) \| ZF   | Less or Equal (Signed)    |
| ja   | ~CF & ~ZF         | Above (Unsigned)          |
| jb   | CF                | Below (Unsigned)          |

#### Conditional Branch Example

```C
long absdiff(long x, long y)
{
  long result;
  if (x > y)
	  result = x-y;
  else
  	result = y-x;
  return result;
}
```

##### Old Style

Generation: `gcc –Og -S –fno-if-conversion control.c`, where `-fno-if-conversion` means do not use conditional move.

```assembly
absdiff:
	cmpq		%rsi,	%rdi	# x:y
	jle			.L4
	movq		%rdi,	%rax
	subq		%rsi,	%rax
	ret
.L4:			# x <= y
	movq		%rsi,	%rax
	subq		%rdi,	%rax
	ret
```

##### Conditional move

```assembly
absdiff:
	movq		%rdi,	%rax	# x
	subq		%rsi,	%rax	# result = x - y
	movq		%rsi,	%rdx	# y
	subq		%rdi,	%rdx	# eval = y - x
	cmpq		%rsi,	%rdi	# x:y
	comvle	%rdx,	%rax	# if <=, result = eval
	ret
```

You can find that, using conditional move can reduce conditional jump, which delays the pipeline.

Conditional move is not always the best choice. You should know conditional move does the both computations, and decide which one is the real one before return. But sometimes, it might be a really bad idea to do both computations:

- Expensive computations: `val = Test(x) ? Hard1(x) : Hard2(x);`
- Risky computations: `val = p ? *p : 0;`
- Computations with side effects: `val = x > 0 ? x *= 7 : x += 3;`

### Loops

#### Do-While

Do-While loop is the most basic and the easiest loop format, although we rarely use it.

**C Code**:

```C
do
  Body
while (Test);
```

**Goto Version**:

```C
loop:
  Body
  if (Test)
    goto Loop
```

You can translate goto version to assembly directly.

#### While

**C Code**:

```C
while (Test)
  body
```

**Goto Version 1 (Jump-to-middle translation)**:

Optimization parameter: `-Og`

```C
goto Test;
Loop:
  Body
Test:
  if (Test)
    goto Loop;
```

You can find that, this version just add `goto Test` part based on the goto version of do-while loop. It just fixes the difference between the two loop.

**Goto Version 2 (Do-while translation)**:

Optimization parameter: `-O1`

```C
if (!Test)
  goto Done:
Loop:
	Body
Test:
	if (Test)
    goto Loop;
Done:
```

Actually, this one is only testing first by myself, and the other one is testing by `goto Test`. So why this one is the result by `-O1`? This is because that, at most time, we initialize before we do loop, and at most time, the initialized variables can pass the test. So when we use the corresponding assembly code of this goto version, the initial test can *be optimized away* by compiler.

#### For

**C Code**

```C
for (Init; Test; Update)
  Body
```

**While Version**

```C
Init;
while (Test) {
  Body
  Update;
}
```

You can find the `-O1` optimization from:

![Optimize away initial test](./figures/machine_control_optimize_away_test.png)

### Switch Statements

The compiler would build a jump table, where every cases map a code block. This helps `switch-case` statement do in *O(1)* time. The slides about here is all important I think, so I recommend you to read the slides, and I'm not going to copy them all here.

![Switch Statement Example in High Level](./figures/machine_control_switch.png)

But here's some tricky things when compiler compiles `switch-case` statements:

- In the example, the all cases are `1, 2, 3, 5, 6`. So the compiler will compare `x` with 6 **in unsigned format**. If `x > 6`, which means `x < 0` or `x > 6` in signed format, the program will jump to the default code block (`L8`). And map `0` and `4` to `L8` in jump table.
- If the all cases contains negative number or starts from a larger number which is not near 0, the compiler will **put some bias** so that the first case is 0.
- If it's a really big spread of cases and relatively sparse, the compiler will **revert to `if-else` code in halfway through on average (binary search)**, which can do in *logarithmic* time.

## Lecture 7: Machine-Level Programming III: Procedures

>  Create Time: 2023.08.21  Update Time: 2023.08.21

### Mechanisms in Procedures

Break procedures into some small settles.

+ Passing control
  + To beginning of procedure code
  + Back to return point
+ Passing data
  + Procedure arguments
  + Return value
+ Memory management
  + Allocate during procedure execution
  + Deallocate upon return

### Stack Structure

> Stack is the right data structure for procedure call/return

To the assembly-level programmers' perspective, memory is just a big array of bytes. And somewhere within that bunch of bytes, we're going to call it the stack. And the stack is used by the program to manage the state associated with the procedures that it calls and as they return.

`%rsp` is the stack pointer. When more data are allocated for the stack, stack pointer decrements.

+ `pushq src`
+ `popq src`

### Calling Conventions

#### Procedure Control Flow

Use stack to support procedure call and return

+ Procedure call: `call label`
  + Push return address (address of the next instruction right after call) on stack
  + Jump to label
+ Procedure return: `ret`
  + Pop address from stack
  + Jump to address

#### Passing Data

+ First 6 integer and pointer arguments: `%rdi`, `%rsi`, `%rdx`, `%rcx`, `%r8`, `%r9`
  + Put the arguments part more than 6 in memory on the stack.
  + Only allocate stack space when needed.
+ Return value: `%rax`

#### Managing local data

Each block we use for a particular call is called the **stack frame**.

Base pointer/frame pointer is `%rbp`, which is an optional pointer. For most programs, compiler will compute what size of a stack frame a function needs, and allocate stack frame to the function. `%rbp` is mostly only used when a function try to allocate an array with variable size.

#### Register Saving Conventions

+ Caller Saved
  + Caller saves temporary values in its frame before the call
  + `%rax`: Return value
  + `%rdi`, `%rsi`, `%rdx`, `%rcx`, `%r8`, `%r9`: Arguments
  + `%r10`, `%r11`: Caller-saved temporaries
+ Callee saved
  + Callee saves temporary values in its frame before using
  + Callee restores them before returning to caller
  + `%rbx`, `%r12`, `%r13`, `%r14`: Callee-saved temporaries
  + `%rbp`: May be used as frame pointer
  + `%rsp`: Special form of callee save, and it restored to original value upon exit from procedure

### Illustration of Recursion

The stack principle makes recursion work, even mutual recursion.

## Lecture 8: Machine-Level Programming IV: Data

> Create Time: 2023.08.22  Update Time: 2023.08.24

> I don't like the video of this lecture, because I can not access too many hand-writing content by the teacher.

### Arrays

#### One-dimensional: `T A[L];`

**Array Allocation**

+ Array of data type `T` and length `L`
+ Contiguously allocated region of `L * sizeof(T)` bytes in memory

**Array Access**

+ Array of dta type `T` and length`L`
+ Identifier `A` can be used as a pointer to array element 0: `Type T*`
+ `(%rdi, %rsi, 4)`: `%rdi + %rsi * 4`

#### Multi-dimensional (nested): `T A[R][C];`

+ 2D array of data type `T`
+ `R` rows, `C` columns
+ Array Size: `R * C * sizeof(T)`
+ Arrangement: Row-major Ordering

**Row Vectors**

+ `A[i]` is array of `C` elements
+ Starting address `A + i * (C * sizeof(T))`

**Array Elements**

+ `A[i][j]` is element of type `T`
+ Address `A + i * (C * sizeof(T)) + j * sizeof(T) = A + (i * C + j) * sizeof(T)`

#### Multi-Level Array

It means an array pointer pointing some arrays. For example, `int *univ[3] = {mit, cmu, ucb}`.

The difference between nested array and multi-level array is that, nested array does only one memory reference, but multi-level array does twice, for a two-dimensional array. Because multi-level array need do one memory reference to get the address of the low-level array first.

### Structure

#### Representation

+ Structure represented as block of memory, which is big enough to hold all of the fields.
+ Fields ordered according to declaration, even if another ordering could yield a more compact representation.
+ Machine-level program has no understanding of the structures in the source code, and the compiler determines overall size + positions of fields.

#### Alignment Principles

**Aligned Data**

+ Primitive data type requires `K` bytes
+ Address must be multiple of `K`

**Motivation for Aligning Data**

+ Memory accessed by (aligned) chunks of 4 or 8 bytes (system dependent)

**Compiler**

+ Insert gaps in structure to ensure correct alignment of fields

#### Alignment with Structures

**Within structure**

+ Must satisfy each element's alignment requirement

**Overall structure placement**

+ Each structure has alignment requirement `K`, which is the largest alignment of any element
+ Initial address and structure length must be multiples of `K`.

#### Saving Space

A useful method to save space (e.g., make less useless space for alignment) is putting large data types first.

### Floating Point

#### Basics

+ Arguments passed in `%xmm0`, `%xmm`, ...
+ Result returned in `%xmm0`
+ All XMM registers are caller-saved.

#### Memory Referencing

+ Integer and pointer arguments passed in regular registers
+ Floating point values passed in XMM registers
+ Different `mov` instructions to move between XMM registers, and between memory and XMM registers

#### Others

+ Lots of instructions
+ Floating-point comparisons
+ Various tricks to use constant values

## Lecture 9: Machine-Level Programming V: Advanced Topics

>  Create Time: 2023.08.25  Update Time: 2023.08.26

### Memory Layout

`0000_0040_0000_0000` ~  `0000_7FFF_FFFF_FFFF`: 47bits limit.

You can use `limit` command to check some limit.

From highest address to lowest address, there are, Stack, Shared Libraries, Heap, Data and Text.

An interesting program for showing memory layout:

```C
#include <stdio.h>
#include <stdlib.h>

char big_array[1L << 24];   /* 16 MB */
char huge_array[1L << 31];  /* 2 GB */

int global = 0;

int useless() { return 0; }

int main()
{
    void *p1, *p2, *p3, *p4;
    int local = 0;
    p1 = malloc(1L << 28);  /* 256 MB */
    p2 = malloc(1L << 8);   /* 256 B */
    p3 = malloc(1L << 32);  /* 4 GB */
    p4 = malloc(1L << 8);   /* 256 B */
    /* Print */
    printf("Address of local:\t\t%p\n", &local);
    // printf("Address of global:\t\t%p\n", &global);
    
    printf("Address of p1:\t\t\t%p\n", &p1);
    printf("Address of p2:\t\t\t%p\n", &p2);
    printf("Address of p3:\t\t\t%p\n", &p3);
    printf("Address of p4:\t\t\t%p\n", &p4);

    printf("Address of big_array:\t\t%p\n", &big_array);
    printf("Address of huge_array:\t\t%p\n", &huge_array);

    printf("Address of main:\t\t%p\n", main);
    printf("Address of useless:\t\t%p\n", useless);

    return 0;
}
```

### Buffer Overflow

Recall the codes of **Great Reality #3** in Lecture 1, and there is a potential buffer overflow bug.

#### Most common form

+ Unchecked lengths on string inputs
  + `gets`
  + `strcpy`, `strcat`
  + `scanf`,`fscanf`,`sscanf`
+ Particularly for bounded character arrays on the stack
  + Sometimes referred to as stack smashing

#### Code Injection Attacks

+ Input string contains byte representation of executable code
+ Overwrite return address A with address of buffer B
+ When Q executes `ret`, it will jump to exploit code

#### Protection

1. Avoid Overflow Vulnerabilities in Code.
   + `fgets` instead of `gets`
   + `strncpy` instead of `strcpy`
   + Use `fgets` to read the string, or use `%ns` where `n` is a suitable integer.
2. System-Level Protections
   + Randomized stack offsets
     + ASLR (Address Space Layout Randomization)
   + Nonexecutable code segments
3. Stack Canaries

#### Return-Oriented Programming Attacks

**Construct program from *gadgets***

+ Sequence of instructions ending in `ret`, which encoded by single byte `0xc3`
+ Code positions fixed from run to run
+ Code is executable

### Unions

+ Allocate according to largetst element
+ Can only use one field at a time

**Byte Ordering**

```C
union {
  unsigned char c[8];
  unsigned short s[4];
  unsigned int i[2];
  unsigned long l[1];
} dw;
```

## Recitation: Bomb Lab 

> Create Time: 2023.08.29  Update Time: 2023.09.06

> I'm focusing on noting for Activity 3.

You can solve this lab by reading the definition of func `compare` in code file `act3.s`. I'm not familiar with assembly code, so I need `gdb` to help me. But here I record the solution of this lab by comments in assembly code.

```assembly
compare:
.LFB0:
    .cfi_startproc
    pushq     %rbx						/* Record the return address */
    movq      %rdi, %rbx			/* %rbx: num1(%rdi) */
    addq      $5, %rbx				/* %rbx: num1 + 5 */
    addq      %rsi, %rbx			/* %rbx: num1 + 5 + num2 */
    cmpq      $15213, %rbx		/* Compare %rbx(num1 + 5 + num2) and 15213 */
    sete      %al							/* Set ZF and CF flags */
    movzbq    %al, %rax				/* the result of comparation, 1 if equal and 0 if not */
    popq      %rbx						/* Pop the return address */
    ret												/* Return, and the return value is %rax(the comparation result) */
    .cfi_endproc
```

So we can get the solution is input two numbers whose sum is `15213 - 5 = 15208`. Here's my test:

```bash
root@d5661d8d55f2:/home/csapp/labs/bomblab# ./act3 15213 -5
good args!
root@d5661d8d55f2:/home/csapp/labs/bomblab# ./act3 15212 -4
good args!
root@d5661d8d55f2:/home/csapp/labs/bomblab# ./act3 15210 -2
good args!
root@d5661d8d55f2:/home/csapp/labs/bomblab# ./act3 15208 0
good args!
root@d5661d8d55f2:/home/csapp/labs/bomblab# ./act3 15200 8
good args!
```

## Lab: Bomb Lab

> Create Time: 2023.09.07  Update Time: 2023.09.07

Here's the solution of bomb lab. Sometimes, there's many strings for defusing one bomb.

I use `objdump -d bomb > obj.txt` to get the assembly code for bomb file, which I think can help me a lot. I can get the function for explode bombs. Take bomb 1/phase 1 as an example, I can get `phase_1(input);` to handle the phase 1, and then I can get related assembly code from `obj.txt`, where I can find a function naming `explode_bomb`. So I will `break explode_bomb` first to prevent exploding bombs.

### Bomb 1

> `break explode_bomb` and `break phase_1` first and then `run`.

After type `123456` the program paused by breakpoint. We can get assembly code by command `disas` or file `obj.txt`. We find:

```assembly
400ee4:	be 00 24 40 00       	mov    $0x402400,%esi
400ee9:	e8 4a 04 00 00       	callq  401338 <strings_not_equal>
400eee:	85 c0                	test   %eax,%eax
400ef0:	74 05                	je     400ef7 <phase_1+0x17>
400ef2:	e8 43 05 00 00       	callq  40143a <explode_bomb>
400ef7:	48 83 c4 08          	add    $0x8,%rsp
```

This part code means, if the string we put is equal the one in program, we can jump over the `explode_bomb`. From the previous lectures, we can know the registers for the first two parameters is `%rdi` and `%rsi`, and we know the relation between `%rsi` and `%esi`. From this, we can guess the target string is at `0x402400`. And we get it by command `x/s 0x402400`, and it's "`Border relations with Canada have never been better.`". Run again in gdb (because the breakpoint at `explode_bomb` can help me), type this string, and we find we defuse the first bomb.

> I save my answers in `ans.txt`, and I can use `./bomb ans.txt` or `run ans.txt` in gdb to pass the bombs I have defused.

### Bomb 2

When reading assembly code of `phase_2` function, we found it calls `read_six_numbers` first. So we guess we need to input six number. Actually, we can read assembly code of `read_six_numbers` and we can find a memory address `0x4025c3` for `sscanf` call, where "`%d %d %d %d %d %d`" stays. So we need input six integers. I input `1 2 3 4 5 6` for test.

Here's assembly code for `read_six_numbers`:

```assembly
000000000040145c <read_six_numbers>:
  40145c:	48 83 ec 18          	sub    $0x18,%rsp
  401460:	48 89 f2             	mov    %rsi,%rdx
  401463:	48 8d 4e 04          	lea    0x4(%rsi),%rcx
  401467:	48 8d 46 14          	lea    0x14(%rsi),%rax
  40146b:	48 89 44 24 08       	mov    %rax,0x8(%rsp)
  401470:	48 8d 46 10          	lea    0x10(%rsi),%rax
  401474:	48 89 04 24          	mov    %rax,(%rsp)
  401478:	4c 8d 4e 0c          	lea    0xc(%rsi),%r9
  40147c:	4c 8d 46 08          	lea    0x8(%rsi),%r8
  401480:	be c3 25 40 00       	mov    $0x4025c3,%esi
  401485:	b8 00 00 00 00       	mov    $0x0,%eax
  40148a:	e8 61 f7 ff ff       	callq  400bf0 <__isoc99_sscanf@plt>
  40148f:	83 f8 05             	cmp    $0x5,%eax
  401492:	7f 05                	jg     401499 <read_six_numbers+0x3d>
  401494:	e8 a1 ff ff ff       	callq  40143a <explode_bomb>
  401499:	48 83 c4 18          	add    $0x18,%rsp
  40149d:	c3                   	retq
```

Why do we need to understand the function to read our inputs instead of testing the `phase_2` function directly? Becase if we want to test `phase_2` better, we need to know where out inputs go first.

We find it allocate `0x18 = 24 = 6 * 4` bytes for six integers at the top of stack by `sub $0x18, %rsp`. Referenced by `sscanf` format `int sscanf(const char *str, const char *format, ...)`, we know we should find the target address by `%rdx`, `%rcx`, `%r8`, `%r9`. Noted the command `mov %rsp,%rsi` before calling `read_six_numbers`, we can know that, `%rdx` is pointing the 4 bytes at the bottom of this 24 bytes space, `%rcx` is pointing the next one, and `%r8` and `%r9` follow them. Where are the last two integers? Look at `lea 0x10(%rsi),%rax` and `mov %rax,(%rsp)`, they mean the fifth integers will be saved at `0x10(%rsi)`, which is followed by the fourth integers. If you're a little confused, you may be review the part of Passing Data in Lecture 07. Now we know the six numbers are saved at the stack, and on the frame of `phase_2`.

Back to `phase_2`, we get the first bomb check:

```assembly
400f0a:	83 3c 24 01          	cmpl   $0x1,(%rsp)
400f0e:	74 20                	je     400f30 <phase_2+0x34>
400f10:	e8 25 05 00 00       	callq  40143a <explode_bomb>
```

This mean if the integer `%rsp` pointing (i.e., the first integer I input) is not `0x1`, the bomb is exploded. Luckily, the first integer I input is `1`, so I pass this check. Now I focus on the following bomb check, and I will explain the code by comments:

```assembly
0x0000000000400f05 <+9>:     callq  0x40145c <read_six_numbers>	# read six integers
0x0000000000400f0a <+14>:    cmpl   $0x1,(%rsp)									# if the first one is 1
0x0000000000400f0e <+18>:    je     0x400f30 <phase_2+52>				# if it is, jump to PHASE_2+52
0x0000000000400f10 <+20>:    callq  0x40143a <explode_bomb>
0x0000000000400f15 <+25>:    jmp    0x400f30 <phase_2+52>
0x0000000000400f17 <+27>:    mov    -0x4(%rbx),%eax							# %eax is the previous integer
0x0000000000400f1a <+30>:    add    %eax,%eax										# double %eax
0x0000000000400f1c <+32>:    cmp    %eax,(%rbx)									# if the integer is equal to double previous one
0x0000000000400f1e <+34>:    je     0x400f25 <phase_2+41>				# if it is, jump
0x0000000000400f20 <+36>:    callq  0x40143a <explode_bomb>
0x0000000000400f25 <+41>:    add    $0x4,%rbx										# move %rbx to next one
0x0000000000400f29 <+45>:    cmp    %rbp,%rbx										# if the all integers are checked?
0x0000000000400f2c <+48>:    jne    0x400f17 <phase_2+27>				# if not, go on
0x0000000000400f2e <+50>:    jmp    0x400f3c <phase_2+64>				# if all are checked, finished.
0x0000000000400f30 <+52>:    lea    0x4(%rsp),%rbx							# %rbx is address of the second integer
0x0000000000400f35 <+57>:    lea    0x18(%rsp),%rbp							# %rbp is the end address of all integers
0x0000000000400f3a <+62>:    jmp    0x400f17 <phase_2+27>
0x0000000000400f3c <+64>:    add    $0x28,%rsp
0x0000000000400f40 <+68>:    pop    %rbx
0x0000000000400f41 <+69>:    pop    %rbp
0x0000000000400f42 <+70>:    retq
```

Now we know need to input six numbers, the first one of which is must be `1` and the every integer is double of the previous one. So the answer is `1 2 4 8 16 32`.

### Bomb 3

As before, I can easily find I need to input 2 integers by `x/s 0x4025cf`, and the 2 integers will be saved at `0x8(%rsp)` and `0xc(%rsp)`.

```assembly
0x0000000000400f43 <+0>:     sub    $0x18,%rsp
0x0000000000400f47 <+4>:     lea    0xc(%rsp),%rcx						# the second one
0x0000000000400f4c <+9>:     lea    0x8(%rsp),%rdx						# the first one
0x0000000000400f51 <+14>:    mov    $0x4025cf,%esi
0x0000000000400f56 <+19>:    mov    $0x0,%eax
0x0000000000400f5b <+24>:    callq  0x400bf0 <__isoc99_sscanf@plt>
0x0000000000400f60 <+29>:    cmp    $0x1,%eax
0x0000000000400f63 <+32>:    jg     0x400f6a <phase_3+39>
0x0000000000400f65 <+34>:    callq  0x40143a <explode_bomb>
0x0000000000400f6a <+39>:    cmpl   $0x7,0x8(%rsp)						# compare the first one and 7
0x0000000000400f6f <+44>:    ja     0x400fad <phase_3+106>		# if the integer > 7, explode the bomb.
0x0000000000400f71 <+46>:    mov    0x8(%rsp),%eax
0x0000000000400f75 <+50>:    jmpq   *0x402470(,%rax,8)				# jump to the address stored in the target address
0x0000000000400f7c <+57>:    mov    $0xcf,%eax
0x0000000000400f81 <+62>:    jmp    0x400fbe <phase_3+123>
0x0000000000400f83 <+64>:    mov    $0x2c3,%eax
0x0000000000400f88 <+69>:    jmp    0x400fbe <phase_3+123>
0x0000000000400f8a <+71>:    mov    $0x100,%eax
0x0000000000400f8f <+76>:    jmp    0x400fbe <phase_3+123>
0x0000000000400f91 <+78>:    mov    $0x185,%eax
0x0000000000400f96 <+83>:    jmp    0x400fbe <phase_3+123>
0x0000000000400f98 <+85>:    mov    $0xce,%eax
0x0000000000400f9d <+90>:    jmp    0x400fbe <phase_3+123>
0x0000000000400f9f <+92>:    mov    $0x2aa,%eax
0x0000000000400fa4 <+97>:    jmp    0x400fbe <phase_3+123>
0x0000000000400fa6 <+99>:    mov    $0x147,%eax
0x0000000000400fab <+104>:   jmp    0x400fbe <phase_3+123>
0x0000000000400fad <+106>:   callq  0x40143a <explode_bomb>
0x0000000000400fb2 <+111>:   mov    $0x0,%eax
0x0000000000400fb7 <+116>:   jmp    0x400fbe <phase_3+123>
0x0000000000400fb9 <+118>:   mov    $0x137,%eax
0x0000000000400fbe <+123>:   cmp    0xc(%rsp),%eax
0x0000000000400fc2 <+127>:   je     0x400fc9 <phase_3+134>
0x0000000000400fc4 <+129>:   callq  0x40143a <explode_bomb>
0x0000000000400fc9 <+134>:   add    $0x18,%rsp
0x0000000000400fcd <+138>:   retq
```

It's notable that the command `jmpq *0x402470(,%rax,8)` means jump to the address stored in the `%rax * 8 + 0x402470`. We can print the values near 0x402470 by `x/8g 0x402470`:

```assembly
(gdb) x/8g 0x402470
0x402470:       0x0000000000400f7c      0x0000000000400fb9
0x402480:       0x0000000000400f83      0x0000000000400f8a
0x402490:       0x0000000000400f91      0x0000000000400f98
0x4024a0:       0x0000000000400f9f      0x0000000000400fa6
```

 You can find it is referred to the command in `phase_3`. Now I realize it may be a switch-case, and I can get the corresponding relationship as follow:

| Key  | Value |
| :--: | :---: |
|  0   | 0xcf  |
|  1   | 0x137 |
|  2   | 0x2c3 |
|  3   | 0x100 |
|  4   | 0x185 |
|  5   | 0xce  |
|  6   | 0x2aa |
|  7   | 0x147 |

If you want to defuse the bomb more quickly, you can just input an integer which is smaller than 7, and pause it by breakpoint, find the corresponding target integer by using `stepi` command or `nexti` command.

### Bomb 4

I can also easily find that I need two integers as input, and I input `1 2` for test. Here's the assembly code for `phase_4`.

```assembly
0x000000000040100c <+0>:     sub    $0x18,%rsp
0x0000000000401010 <+4>:     lea    0xc(%rsp),%rcx
0x0000000000401015 <+9>:     lea    0x8(%rsp),%rdx
0x000000000040101a <+14>:    mov    $0x4025cf,%esi
0x000000000040101f <+19>:    mov    $0x0,%eax
0x0000000000401024 <+24>:    callq  0x400bf0 <__isoc99_sscanf@plt>
0x0000000000401029 <+29>:    cmp    $0x2,%eax
0x000000000040102c <+32>:    jne    0x401035 <phase_4+41>
0x000000000040102e <+34>:    cmpl   $0xe,0x8(%rsp)
0x0000000000401033 <+39>:    jbe    0x40103a <phase_4+46>
0x0000000000401035 <+41>:    callq  0x40143a <explode_bomb>
0x000000000040103a <+46>:    mov    $0xe,%edx
0x000000000040103f <+51>:    mov    $0x0,%esi
0x0000000000401044 <+56>:    mov    0x8(%rsp),%edi
0x0000000000401048 <+60>:    callq  0x400fce <func4>
0x000000000040104d <+65>:    test   %eax,%eax
0x000000000040104f <+67>:    jne    0x401058 <phase_4+76>
0x0000000000401051 <+69>:    cmpl   $0x0,0xc(%rsp)
0x0000000000401056 <+74>:    je     0x40105d <phase_4+81>
0x0000000000401058 <+76>:    callq  0x40143a <explode_bomb>
0x000000000040105d <+81>:    add    $0x18,%rsp
0x0000000000401061 <+85>:    retq
```

I notice `lea 0x8(%rsp),%rdx`, `cmpl $0xe,0x8(%rsp)` and `jbe 0x40103a <phase_4+46>`, so I need the first integer less than or equal to `0xe` and I can move onto `<phase_4+46>`.

Before it calls `func4`, `%rdi` is assigned as my first integer, `%rsi` is assigned as 0, `%rdx` is assigned as `0xe = 14`. Now let me focus on `func4` (it calls itself, i.e., Recursion):

```assembly
0x0000000000400fce <+0>:     sub    $0x8,%rsp
0x0000000000400fd2 <+4>:     mov    %edx,%eax								# %eax = %edx = 14
0x0000000000400fd4 <+6>:     sub    %esi,%eax								# %eax = %eax - %esi = 14
0x0000000000400fd6 <+8>:     mov    %eax,%ecx								# %ecx = 14
0x0000000000400fd8 <+10>:    shr    $0x1f,%ecx							# shift right 0x1f with 14 bits, and %ecx get 0
0x0000000000400fdb <+13>:    add    %ecx,%eax								# %eax = %eax + %ecx = 14
0x0000000000400fdd <+15>:    sar    %eax										# shift right %eax with 1 bits, %eax = 7
0x0000000000400fdf <+17>:    lea    (%rax,%rsi,1),%ecx			# %ecx = %rax + %rsi = 7
0x0000000000400fe2 <+20>:    cmp    %edi,%ecx								# cmp my first int and 7
0x0000000000400fe4 <+22>:    jle    0x400ff2 <func4+36>			# jump if my first int >= 7 ------- case 1
0x0000000000400fe6 <+24>:    lea    -0x1(%rcx),%edx					# %edx = %rcx - 1 = 6
0x0000000000400fe9 <+27>:    callq  0x400fce <func4>				# call func4
0x0000000000400fee <+32>:    add    %eax,%eax
0x0000000000400ff0 <+34>:    jmp    0x401007 <func4+57>
0x0000000000400ff2 <+36>:    mov    $0x0,%eax								# %eax = 0
0x0000000000400ff7 <+41>:    cmp    %edi,%ecx
0x0000000000400ff9 <+43>:    jge    0x401007 <func4+57>			# jump if my first int <= 7
0x0000000000400ffb <+45>:    lea    0x1(%rcx),%esi
0x0000000000400ffe <+48>:    callq  0x400fce <func4>
0x0000000000401003 <+53>:    lea    0x1(%rax,%rax,1),%eax
0x0000000000401007 <+57>:    add    $0x8,%rsp
0x000000000040100b <+61>:    retq
```

When I analysis what the codes mean, I find, if my first integer is equal to 7, I can just pass the `func4` without recursion and get a return value `0`.

Back to `phase_4`, I can read it and I know I need `func4` to return `0` and the second integer is `0`. So the first answer is `7 0`.

I want to know what happen in `func4`. `func4` needs 3 parameters, let me name them `a`, `b` and `c`. Here's what I get from assembly code:

```C
func4(a, b, c):	// a is my int, b = 0 and c = 14 by default
  d = c - b;
  e = d >> 0x1f;// e = 0
  e += d;				// e = d
  e = e >> 1;		// e is 7 if b = 0 and c = 14
  if (e + b <= a)
    if (e + b >= a)
      return 0;
    else
      f = func4(a, e + b + 1, c)
      return 2 * f + 1;
  else
    f = func4(a, b, e + b - 1);
		return 2 * f;
```

So I can find `7`, `3`, `1`, `0` are OK for the first integer. Actually, `a` must not be greater than `7`, and then `b` is always `0`, `e = c / 2`. The condition is `a == c/2`.

### Bomb 5

After defusing 4 bombs, I can find that I need input a string with 6 characters easily.

I notice command `movzbl 0x4024b0(%rdx),%edx`, and I get `maduiersnfotvbylSo you think you can stop the bomb with ctrl-c, do you?` by `x/s 0x4024b0`. At the same time, I alse notice command `mov $0x40245e,%esi` and I get `flyers` by `x/s 0x40245e`. And after I pass the function compare 2 strings, I can defuse the bomb. So I need to know what `phase_5` do on my strings.

As my comments show, `%rdx` is the index of string in `0x4024b0`, and only the lowest 4 bits is useful. The indices of `f`, `l`, `y`, `e`, `r`, `s` are `0x9`, `0xf`, `0xe`, `0x5`, `0x6`, `0x7`. So I just find a string, the lowest 4 bits of whose chars is corresponding to these numbers, and I can defuse. And I chose `9?>567`.

```assembly
0x0000000000401062 <+0>:     push   %rbx
0x0000000000401063 <+1>:     sub    $0x20,%rsp
0x0000000000401067 <+5>:     mov    %rdi,%rbx
0x000000000040106a <+8>:     mov    %fs:0x28,%rax									# Stack Corruption Detection
0x0000000000401073 <+17>:    mov    %rax,0x18(%rsp)
0x0000000000401078 <+22>:    xor    %eax,%eax
0x000000000040107a <+24>:    callq  0x40131b <string_length>
0x000000000040107f <+29>:    cmp    $0x6,%eax											# 6 characters
0x0000000000401082 <+32>:    je     0x4010d2 <phase_5+112>
0x0000000000401084 <+34>:    callq  0x40143a <explode_bomb>
0x0000000000401089 <+39>:    jmp    0x4010d2 <phase_5+112>
0x000000000040108b <+41>:    movzbl (%rbx,%rax,1),%ecx						# %rbx is my strings, %rax means index of char
0x000000000040108f <+45>:    mov    %cl,(%rsp)
0x0000000000401092 <+48>:    mov    (%rsp),%rdx
0x0000000000401096 <+52>:    and    $0xf,%edx											# keep the lowest 4 bits of %rdx
0x0000000000401099 <+55>:    movzbl 0x4024b0(%rdx),%edx						# %rdx is index of 0x4024b0
0x00000000004010a0 <+62>:    mov    %dl,0x10(%rsp,%rax,1)
0x00000000004010a4 <+66>:    add    $0x1,%rax											# for each char in my strings
0x00000000004010a8 <+70>:    cmp    $0x6,%rax
0x00000000004010ac <+74>:    jne    0x40108b <phase_5+41>
0x00000000004010ae <+76>:    movb   $0x0,0x16(%rsp)
0x00000000004010b3 <+81>:    mov    $0x40245e,%esi								# string "flyers" is the 2nd param for strings_not_equal
0x00000000004010b8 <+86>:    lea    0x10(%rsp),%rdi
0x00000000004010bd <+91>:    callq  0x401338 <strings_not_equal>
0x00000000004010c2 <+96>:    test   %eax,%eax
0x00000000004010c4 <+98>:    je     0x4010d9 <phase_5+119>
0x00000000004010c6 <+100>:   callq  0x40143a <explode_bomb>
0x00000000004010cb <+105>:   nopl   0x0(%rax,%rax,1)
0x00000000004010d0 <+110>:   jmp    0x4010d9 <phase_5+119>
0x00000000004010d2 <+112>:   mov    $0x0,%eax
0x00000000004010d7 <+117>:   jmp    0x40108b <phase_5+41>
0x00000000004010d9 <+119>:   mov    0x18(%rsp),%rax
0x00000000004010de <+124>:   xor    %fs:0x28,%rax
0x00000000004010e7 <+133>:   je     0x4010ee <phase_5+140>
0x00000000004010e9 <+135>:   callq  0x400b30 <__stack_chk_fail@plt>
0x00000000004010ee <+140>:   add    $0x20,%rsp
0x00000000004010f2 <+144>:   pop    %rbx
0x00000000004010f3 <+145>:   retq
```

### Bomb 6

> Because the assembly code of `phase_6` is too long, I'm not going to put all them here. I'll try to explain my thoughts about defusing the bomb by notes.

It's obvious that I need input six integers because of the `read_six_numbers` function call. As analysis for `phase_2`, my six integers are going to be put atop of the frame of `phase_6`.

#### Input

After `read_six_numbers`, we move on

```assembly
0x0000000000401117 <+35>:    mov    0x0(%r13),%eax
0x000000000040111b <+39>:    sub    $0x1,%eax
0x000000000040111e <+42>:    cmp    $0x5,%eax
0x0000000000401121 <+45>:    jbe    0x401128 <phase_6+52>
0x0000000000401123 <+47>:    callq  0x40143a <explode_bomb>
0x0000000000401128 <+52>:    add    $0x1,%r12d
```

Now here `%r13` is represented as the top of the stack (frame of `phase_6`), so this part of the code is checking the integer `%r13` pointing (now it's the first one) is not larger than 6. Because it use `jbe` to check, I can use `0` as my input (`jbe` is for unsigned int, `0u - 1 = UMax > 6`. Let's move on.

#### Input Check

```assembly
0x000000000040110e <+26>:    mov    $0x0,%r12d
...
0x0000000000401128 <+52>:    add    $0x1,%r12d
0x000000000040112c <+56>:    cmp    $0x6,%r12d
0x0000000000401130 <+60>:    je     0x401153 <phase_6+95>
```

So I know `%r12d` is a counter from 1 to 5 (included), and I guess it corresponds to the indices of my integers.

```assembly
0x0000000000401132 <+62>:    mov    %r12d,%ebx
0x0000000000401135 <+65>:    movslq %ebx,%rax
0x0000000000401138 <+68>:    mov    (%rsp,%rax,4),%eax
0x000000000040113b <+71>:    cmp    %eax,0x0(%rbp)
0x000000000040113e <+74>:    jne    0x401145 <phase_6+81>
0x0000000000401140 <+76>:    callq  0x40143a <explode_bomb>
0x0000000000401145 <+81>:    add    $0x1,%ebx
0x0000000000401148 <+84>:    cmp    $0x5,%ebx
0x000000000040114b <+87>:    jle    0x401135 <phase_6+65>
0x000000000040114d <+89>:    add    $0x4,%r13
0x0000000000401151 <+93>:    jmp    0x401114 <phase_6+32>
```

This part is checking the six integers are all different, likely using a nested loop. Now I know I need input a permutation of 1, 2, 3, 4, 5, 6.

#### Input Process

After checking, the code goes to `0x401153`:

```assembly
0x000000000040110b <+23>:    mov    %rsp,%r14								# %r14 points the top of the stack
...
0x0000000000401153 <+95>:    lea    0x18(%rsp),%rsi					# %rsi points the last integer
0x0000000000401158 <+100>:   mov    %r14,%rax								# %rax starts from the top of the stack
0x000000000040115b <+103>:   mov    $0x7,%ecx
0x0000000000401160 <+108>:   mov    %ecx,%edx
0x0000000000401162 <+110>:   sub    (%rax),%edx							# %edx = 7 - (%rax)
0x0000000000401164 <+112>:   mov    %edx,(%rax)							# save 7 - (%rax) at the same place
0x0000000000401166 <+114>:   add    $0x4,%rax
0x000000000040116a <+118>:   cmp    %rsi,%rax								# if all the six integers are handled
0x000000000040116d <+121>:   jne    0x401160 <phase_6+108>
```

It's like using a loop for every integers in an array `a` and executing `a[i] = 7 - a[i]`.

#### Ordering

##### Part 1

After the process:

```assembly
0x000000000040116f <+123>:   mov    $0x0,%esi
0x0000000000401174 <+128>:   jmp    0x401197 <phase_6+163>
0x0000000000401176 <+130>:   mov    0x8(%rdx),%rdx					# assign %rdx for loop (details below)
0x000000000040117a <+134>:   add    $0x1,%eax								# %eax (initialized by 1) is must be larger than 1
0x000000000040117d <+137>:   cmp    %ecx,%eax
0x000000000040117f <+139>:   jne    0x401176 <phase_6+130>	# loop until %eax == my int
0x0000000000401181 <+141>:   jmp    0x401188 <phase_6+148>
0x0000000000401183 <+143>:   mov    $0x6032d0,%edx					# the special process for %ecx == 1
0x0000000000401188 <+148>:   mov    %rdx,0x20(%rsp,%rsi,2)	# save the address(%rdx) to %rsp + %rsi(index of processed integer) * 2 + 0x20
0x000000000040118d <+153>:   add    $0x4,%rsi
0x0000000000401191 <+157>:   cmp    $0x18,%rsi
0x0000000000401195 <+161>:   je     0x4011ab <phase_6+183>
0x0000000000401197 <+163>:   mov    (%rsp,%rsi,1),%ecx			# for every int which saved at %ecx
0x000000000040119a <+166>:   cmp    $0x1,%ecx
0x000000000040119d <+169>:   jle    0x401183 <phase_6+143>	# if %ecx <= 1, jump
0x000000000040119f <+171>:   mov    $0x1,%eax								# %eax = 1
0x00000000004011a4 <+176>:   mov    $0x6032d0,%edx					# %edx = 0x6032d0
0x00000000004011a9 <+181>:   jmp    0x401176 <phase_6+130>
```

When I notice `mov $0x0,%esi`, `add $0x4,%rsi` and `cmp $0x18,%rsi`, I realize that `%esi`/`%rsi` is the bias/indices of the stack top for processed integers. I also note that the address `0x6032d0`, but I get a meaningless string `"L\00"`. But I get the name of this address, which is `node1`, and there are six nodes (I use `1 2 3 4 5 6` for input to test):

```assembly
# (gdb) x/24w 0x6032d0
0x6032d0 <node1>:       0x0000014c      0x00000001      0x006032e0      0x00000000
0x6032e0 <node2>:       0x000000a8      0x00000002      0x006032f0      0x00000000
0x6032f0 <node3>:       0x0000039c      0x00000003      0x00603300      0x00000000
0x603300 <node4>:       0x000002b3      0x00000004      0x00603310      0x00000000
0x603310 <node5>:       0x000001dd      0x00000005      0x00603320      0x00000000
0x603320 <node6>:       0x000001bb      0x00000006      0x00000000      0x00000000
```

When it executes `mov 0x8(%rdx),%rdx`, `%rdx` is assigned as the corresponding integers, for example, `0x006032e0` for the first time (node1). `0x40117d` means repeating this assignment 5 times, and the corresponding integers is just the address of next node. So this command is just loopting for the integers.

Let me recall what this part does: for every processed int saved at `%ecx`, loop until `%eax == %ecx` (actually, the `%eax` is index (from 1) of nodes), save the address of the node at `%rsp + %rsi(index of processed integer) * 2 + 0x20` (I think `0x20` is because there's 0x18 bytes for my 6 integers). So this part just copies the address of `node i` to `%rsp + 2 * (index of i) + 0x20`. Take my input (`1 2 3 4 5 6`) as an example, the processed input is `6 5 4 3 2 1`, and the addresses are ,`node6`, `node5`, `node4 `, `node3`, `node2`, `node1`:

```assembly
# (gdb) x/6g ($rsp + 0x20)
0x7fffffffe560: 0x0000000000603320      0x0000000000603310
0x7fffffffe570: 0x0000000000603300      0x00000000006032f0
0x7fffffffe580: 0x00000000006032e0      0x00000000006032d0
```

##### Part 2

> I use `num i` as the integer saved at `node i`.
>
> Be careful, the first node may be not `node1`, and the last node may be not `node6`.

```assembly
0x00000000004011ab <+183>:   mov    0x20(%rsp),%rbx					# 0x20(%rsp): the address of first node, %rbx is address
0x00000000004011b0 <+188>:   lea    0x28(%rsp),%rax					# 0x28(%rsp): the address of second address
0x00000000004011b5 <+193>:   lea    0x50(%rsp),%rsi					# 0x50(%rsp): end of the frame
0x00000000004011ba <+198>:   mov    %rbx,%rcx								# %rcx starts from the first node
# loop starts
0x00000000004011bd <+201>:   mov    (%rax),%rdx							# %rdx is the address saved at (%rax)
0x00000000004011c0 <+204>:   mov    %rdx,0x8(%rcx)					# save the address to %rcx(address) + 0x08, actually, to the position saved next node address originally (details next code block)
0x00000000004011c4 <+208>:   add    $0x8,%rax								# next node
0x00000000004011c8 <+212>:   cmp    %rsi,%rax
0x00000000004011cb <+215>:   je     0x4011d2 <phase_6+222>
0x00000000004011cd <+217>:   mov    %rdx,%rcx								# copy the address to %rcx
0x00000000004011d0 <+220>:   jmp    0x4011bd <phase_6+201>
# loop ends
```

Actually, here's two synchronous loop: one loop on nodes in the stack, the other follow the address saved. The one about addresses is more important.

Here shows the result of this loop. The numbers added `*` are the changed part.

```assembly
# (gdb) x/24w 0x6032d0
0x6032d0 <node1>:       0x0000014c      0x00000001       0x006032e0      0x00000000
0x6032e0 <node2>:       0x000000a8      0x00000002      *0x006032d0      0x00000000
0x6032f0 <node3>:       0x0000039c      0x00000003      *0x006032e0      0x00000000
0x603300 <node4>:       0x000002b3      0x00000004      *0x006032f0      0x00000000
0x603310 <node5>:       0x000001dd      0x00000005      *0x00603300      0x00000000
0x603320 <node6>:       0x000001bb      0x00000006      *0x00603310      0x00000000
```

If you understand it's for a loop before, I believe you can understand it's for another loop now, in an order you input.

##### Part 3

```assembly
0x00000000004011d2 <+222>:   movq   $0x0,0x8(%rdx)					# %rdx is the address of the last node
0x00000000004011da <+230>:   mov    $0x5,%ebp
# loop starts
0x00000000004011df <+235>:   mov    0x8(%rbx),%rax					# %rbx starts from the address of the first node
0x00000000004011e3 <+239>:   mov    (%rax),%eax							# %eax is the num of the address saved at %rax, or you can view it as a pointer.
0x00000000004011e5 <+241>:   cmp    %eax,(%rbx)
0x00000000004011e7 <+243>:   jge    0x4011ee <phase_6+250>	# safe if the num pointed by the previous node >= the num pointed by the next one
0x00000000004011e9 <+245>:   callq  0x40143a <explode_bomb>
0x00000000004011ee <+250>:   mov    0x8(%rbx),%rbx					# next loop
0x00000000004011f2 <+254>:   sub    $0x1,%ebp
0x00000000004011f5 <+257>:   jne    0x4011df <phase_6+235>
# loop ends
```

So now I know I need to sort the numbers, `0x0000014c`, `0x000000a8`, `0x0000039c`, `0x000002b3`, `0x000001dd`, `0x000001bb`, in a decreasing order. The order is 3, 4, 5, 6, 1, 2. I need the node order in the stack is 3, 4, 5, 6, 1, 2. So the processed integers are 3, 4, 5, 6, 1, 2, and my input integers are 4, 3, 2, 1, 6, 5. The answer is `4 3 2 1 6 5`.
