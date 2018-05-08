#include "prelude.h"

// recursive functions
#define triangular(p, ...) if nop2 (top isempty (p), zero, top plus nop32 (p _ triangular pred (p)), __VA_ARGS__)

// once used, no included children can use persist
// otherwise, will accumulate extra copies of included children with each pass
persist
