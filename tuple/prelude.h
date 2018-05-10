/*
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
  (s, ) = 1
  (..., s, ) = n + 1 where n = (...)

  The numbers are defined recursively to the left so that the
  rightmost "empty tuple" in every number can be easily
  discarded for addition.
 
  To be compatible with dup, pop, etc., arithmetic operations
  are defined to act on a tuple of numbers (i.e. a tuple of
  tuples).
*/

// the constant 0
#define zero ()

// add 1 to the top element
#define succ(n, ...) ((s _ n), __VA_ARGS__)

// subtract 1 from the top element
#define pred(n, ...) (pop n, __VA_ARGS__)

// add the top two elements
#define plus(n, m, ...) (($ n $ m), __VA_ARGS__)

/*
  Boolean values are just encoded as the strings 'true' and 'false'.
*/

// yields an error on anything but the empty tuple, leaving false
// thus only expands to true for empty tuple
#define false() true

// is the top item an empty tuple?
#define isempty(x, ...) (false x, __VA_ARGS__)

// is the top item zero?
#define iszero(x, ...) (false x, __VA_ARGS__)

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

// subtract the second element from the first
#define minus(n, m, ...) if nop (top iszero (m), \
  top if nop (top iszero (n), \
    zero, \
    n, \
  ), top minus (top pred (n), top pred (m)), __VA_ARGS__)

// multiply the top two elements together
#define mult(n, m, ...) if nop (top iszero (n), \
  zero, \
  top plus (m, top mult (top pred (n), m)), __VA_ARGS__)

// compute the power n^m
#define pow(n, m, ...) if nop (top iszero (m), \
  (s, ), \
  top mult (n, top pow (n, top pred (m))), __VA_ARGS__)

// is (n <= m)?
#define leq(n, m, ...) iszero nop2 minus (n, m, __VA_ARGS__)

// is (n >= m)?
#define geq(n, m, ...) iszero nop2 minus (m, n, __VA_ARGS__)

// is (n == m)?
#define eq(n, m, ...) if nop (top iszero (m), \
  top if nop (top iszero (n), \
    true, \
    false, \
  ), top eq (top pred (n), top pred (m)), __VA_ARGS__)

// is (n < m)?
#define lt(n, m, ...) if nop (top iszero (n), \
  top if nop (top iszero (m), \
    false, \
    true, \
  ), top lt (top pred (n), top pred (m)), __VA_ARGS__)

// is (n > m)?
#define gt(n, m, ...) lt (m, n, __VA_ARGS__)

/*
  Lists are just tuples.
*/

// get the first item of the list at the top
#define head(xs, ...) (top xs, __VA_ARGS__)

// discard the first item of the list at the top
#define tail(xs, ...) (pop xs, __VA_ARGS__)

// concatenate two lists
#define concat(xs, ys, ...) (($ xs, $ ys), __VA_ARGS__)

/*
  Since "functions" like head, tail, etc are only evaluated when
  immediately followed by a parenthesis, the tokens themselves
  can sort of allow for higher order functions.
*/

// apply f to (x, ...)
#define apply(f, x, ...) f (x, __VA_ARGS__) 

// compose f and g
#define compose(f, g, ...) (f g, __VA_ARGS__)

// apply f to each element in the list l
#define map(f, l, ...) if nop (top isempty (l), \
  (), \
  top if nop2 (top isempty tail (l), \
    (top f l), \
    (top f l, $ top map (f, top tail (l))) \
  ), __VA_ARGS__)

// use f to apply right fold on l with initial element e
#define foldr(f, e, l, ...) if nop4 (top isempty tail (l), \
  top f (top head (l), e), \
  top f (top head (l), top foldr (f, e, top tail (l))), __VA_ARGS__)

// use f to apply left fold on l with initial element e
// (useful when foldr may run out of memory, but slower
// since foldr does multiple expansions per pass while
// foldl must do ~1 application of f per pass)
#define foldl(f, e, l, ...) if nop4 (top isempty tail (l), \
  top f (e, top head (l)), \
  top foldl (f, top f (e, top head (l)), top tail (l)), __VA_ARGS__)

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
