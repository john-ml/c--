#include "prelude.h"

// recursive functions
#define triangular(p, ...) if nop (top iszero (p), zero, top plus nop4 (p _ triangular pred (p)), __VA_ARGS__)

// once used, no included children can use persist
// otherwise, will accumulate extra copies of included children with each pass
persist
