/*
  Inputs: ARG1 ARG2
  Copies a single bit from ARG2 to ARG1.
*/
#include "util.h"

IF ARG2()
  DEFINE ARG1()() 1
ELSE ARG2()
  DEFINE ARG1()() 0
END ARG2()
