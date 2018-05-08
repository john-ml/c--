/*
  Inputs are tuples (a, b, ...) that get applied to
  functions from right to left.

  Most funnctions will take tuples and return tuples.

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

// do nothing
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

// a constant value one
#define zero ()

// add 1 to the top element
#define succ(n, ...) (($ n, s), __VA_ARGS__)

// subtract 1 from the top element
#define pred(n, ...) (($ zero _ pop pop n), __VA_ARGS__)

// add the top two elements
#define plus(n, m, ...) (($ n, $ pop m), __VA_ARGS__)

// force above definitions to persist in future passes
#define wrap(x) x
wrap(#include __FILE__)
