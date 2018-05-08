STACK_SIZE = 10

# make util.h
output = open("util.h", "w")
output.write("""/*
  Abbreviations of commonly used patterns.
*/

// standard directives 
#define __IF__ #if
#define __IFDEF__ #ifdef
#define __IFNDEF__ #ifndef
#define __ELIF__ #elif
#define __ELSE__ #else
#define __END__ #endif
#define __DEFINE__ #define
#define __UNDEF__ #undef
#define __ERROR__ #error
#define __INCLUDE__ #include
#define IF() __IF__ 1 == 
#define IFNOT() __IF__ 1 != 
#define IFDEF() __IFDEF__
#define IFNDEF() __IFNDEF__
#define ELIF() __ELIF__
#define ELSE() __ELSE__
#define END() __END__
#define DEFINE() __DEFINE__
#define UNDEF() __UNDEF__
#define ERROR() __ERROR__
#define INCLUDE() __INCLUDE__

// setting values
#define ON(x) DEFINE x() 1
#define OFF(x) DEFINE x() 0

// get ith bit of x
#define BIT(i, x) x ## _ ## i

#define PERSIST #include __FILE__
PERSIST

""")
output.close()

# make movb.h
output = open("movb.h", "w")

output.write("""/*
  Inputs: ARG1 ARG2
  Copies a single bit from ARG2 to ARG1.
*/
#include "util.h"
""")

output.write("""
IF ARG2()
  DEFINE ARG1()() 1
ELSE ARG2()
  DEFINE ARG1()() 0
END ARG2()
""")

output.close()

# make push.h
output = open("pushb.h", "w")

output.write("""/*
  Inputs: ITEM
  Pushes the bit ITEM onto the stack.
*/
#include "util.h"
""")

elses = 0
for i in reversed(range(STACK_SIZE)):
    output.write("IFNDEF STACK_%d\n" % i)
    output.write("  DEFINE ARG1() STACK_%d\n" % i)
    output.write("  DEFINE ARG2() ITEM\n")
    output.write("  INCLUDE \"movb.h\"\n")
    output.write("ELSE\n")
    elses += 1
    if i == 0:
        output.write("  ERROR Stack overflow\n")
for i in range(elses):
    output.write("END\n")

output.close()

# make pop.h
output = open("popb.h", "w")

output.write("""/*
  Inputs: None
  Pops the bit from the top of the stack into ITEM.
*/
#include "util.h"
""")

elses = 0
for i in range(STACK_SIZE):
    output.write("IFDEF STACK_%d\n" % i)
    output.write("  DEFINE ARG1() ITEM\n")
    output.write("  DEFINE ARG2() STACK_%d\n" % i)
    output.write("  INCLUDE \"movb.h\"\n")
    output.write("  UNDEF STACK_%d\n" % i)
    output.write("ELSE\n")
    elses += 1
    if i == STACK_SIZE - 1:
        output.write("  ERROR Stack underflow\n")
for i in range(elses):
    output.write("END\n")

output.close()

