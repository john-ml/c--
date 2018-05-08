# C--

`c--` is a bash script that repeatedly runs the C/C++ preprocessor `cpp` on a given
input file until the output stops changing.

Usage: `c-- <filename> --verbosity=n`

Verbosity levels:
* `--verbosity=0`: only output the final result
* `--verbosity=1`: also output the result of intermediate passes
* `--verbosity=2`: also output any errors/warnings emitted by `cpp`

## Computing with tuples

The [tuple](https://github.com/johnli0135/c--/blob/master/tuple/) approach uses parameter lists
as tuples to represent data and parametrized macros to represent functions.
(`f(g(x))` is written `f g x`.)

[prelude.h](https://github.com/johnli0135/c--/blob/master/tuple/prelude.h) defines some useful constructs
like stacks, natural numbers, arithmetic, predicates, and conditional execution.

## Computing with low level constructs

The [lowlevel](https://github.com/johnli0135/c--/blob/master/lowlevel/) approach uses lots of variables
to maintain a virtual machine that includes an instruction pointer, a temporary register, and RAM.

[make.py](https://github.com/johnli0135/c--/blob/master/lowlevel/make.py) generates, given a fixed word size
and memory size, preprocessor code responsible for maintaining the state of the virtual machine and loading
and executing instructions from memory.

Programs can be written by directly modifying the initial memory layout. This can be done by including a modified
copy of [init_template.h](https://github.com/johnli0135/c--/blob/master/lowlevel/make.py) with the proper instructions
filled in. For example, the following initialization designed for a VM with word size 4 and memory size 3,
will store the string `1111` in memory address `1` (i.e., `MEM_4` .. `MEM_7`):

```C
/*
  This file defines the initial state of the VM.

  Programs are written by setting values in "main memory" to
  instruction codes and their arguments.
*/

// programs start at address 0
#define IP 0
#define AP 1

// initialize temp register
#define TMP_0 1
#define TMP_1 1
#define TMP_2 1
#define TMP_3 1

// initialize main memory
#define MEM_0 STORE
#define MEM_1 1
#define MEM_2 0
#define MEM_3 0
#define MEM_4 0
#define MEM_5 0
#define MEM_6 0
#define MEM_7 0
#define MEM_8 0
#define MEM_9 0
#define MEM_10 0
#define MEM_11 0
```
