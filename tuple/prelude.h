/*
  Functional approach

  Inputs are tuples (a, b, ...) that get applied to
  functions from right to left.

  Most functions will take tuples and return tuples.

  e.g. f g (1, 2) will apply g to (1, 2), then f to the result.

  We can use this to create a stack machine.
*/

// unpack values
#define $(...) __VA_ARGS__

// cons operator (so (x _ s) pushes an item x onto the "stack" s)
#define _(...) , $(__VA_ARGS__)

// return the top element only
#define top(x, ...) x

// duplicate the top element
#define dup(x, ...) (x, x, __VA_ARGS__)

// discard the top element
#define pop(x, ...) (__VA_ARGS__)

// swap the top two elements
#define swp(x, y, ...) (y, x, __VA_ARGS__)

// send the top element to the end
#define cyc(x, ...) (__VA_ARGS__, x)

/*
  Parent expressions may be evaluated before their children are ready:
    swp(1 _ (2)) will give an error (swp expects 3 args but got 2)
  
  So need to delay evaluation of (1 _ (2)) until it becomes (1, 2)
*/

// do nothing (needed for timing)
#define nop(...) (__VA_ARGS__)
#define nop2 nop nop
#define nop4 nop2 nop2
#define nop8 nop4 nop4
#define nop16 nop8 nop8
#define nop32 nop16 nop16
#define nop64 nop32 nop32
#define nop128 nop64 nop64
#define nop256 nop128 nop128
#define nop512 nop256 nop256
#define nop1024 nop512 nop512

/*
  Tuples can be used to represent natural numbers:
  () = 0
  (..., x) = n + 1 where n = (...)

  The numbers are defined recursively to the left so that the
  empty tuple in every number is always the first element and
  can be easily discarded (e.g. for addition).
 
  To be compatible with dup, pop, etc., arithmetic operations
  are defined to act on a tuple of numbers (i.e. a tuple of
  tuples).
*/

// the constant 0
#define zero ()

// add 1 to the top element
#define succ(n, ...) (($ n, s), __VA_ARGS__)

// add the top two elements
#define plus(n, m, ...) (($ n, $ pop m), __VA_ARGS__)

/*
  Boolean values are just encoded as the strings 'true' and 'false'.
*/

// yields an error on anything but the empty tuple, leaving false
// thus only expands to true for empty tuple
#define false() true

// is the top item an empty tuple?
#define isempty(x, ...) (false x, __VA_ARGS__)

// Church encoding of boolean values
#define iftrue(x, y, ...) (x, __VA_ARGS__)
#define iffalse(x, y, ...) (y, __VA_ARGS__)

// conditional evaluation
#define if(cond, a, b, ...) if ## cond (a, b, __VA_ARGS__)

// logical connectives
#define and(p, q, ...) if (p, top if (q, true, false), false, __VA_ARGS__)
#define or(p, q, ...) if (p, true, top if (q, true, false), __VA_ARGS__)
#define not(p, ...) if (p, false, true, __VA_ARGS__)

/*
  Use conditional expressions to build more complicated arithmetic operators.
*/

// subtract 1 from the top element
#define pred(n, ...) if nop nop nop (top isempty (pop pop n), zero, ($ zero _ pop pop n), __VA_ARGS__)

/*
  The definitions above need to persist across preprocessor passes.
  To do this, define macro to generate an include statement for the current
  file that is only evaluated on the next pass (avoiding circularity).
*/
#define persist #include __FILE__

// if a header file with custom definitions isn't needed, can define
// PERSIST_PRELUDE and prelude.h will persist across passes
#ifdef PERSIST_PRELUDE
  #undef PERSIST_PRELUDE
  #define define_persist_prelude #define PERSIST_PRELUDE
  define_persist_prelude
  persist
#endif