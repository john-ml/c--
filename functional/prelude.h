/*
  Inputs are tuples (a, b, ...) that get applied to
  functions from right to left.

  Most functions will take tuples and return tuples.

  e.g. f g (1, 2) will apply g to (1, 2), then f to the result.

  We can use this to create a stack machine.
*/

// space used to defer evaluation
#define __ 

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

// do nothing (needed for timing)
#define nop(...) (__VA_ARGS__)

// swap the top two elements
#define swp(x, y, ...) (y, x, __VA_ARGS__)

// send the top element to the end
#define cyc(x, ...) (__VA_ARGS__, x)

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

// subtract 1 from the top element
#define pred(n, ...) (($ zero _ pop pop n), __VA_ARGS__)

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

#define f(p, ...) if nop nop nop (top isempty (p), zero, plus nop nop nop nop p f top pred p, __VA_ARGS__)

// force above definitions to persist in future passes
#define wrap(x) x
wrap(#include __FILE__)
