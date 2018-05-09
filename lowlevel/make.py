# This script generates consts.h, init_template.h, vm.h 

MEM_SIZE = 3
WORD_SIZE = 4

# make consts.h
output = open("consts.h", "w")
output.write("""/*
  Instructions and their arguments are encoded as positive integers.
*/

// store MEM[arg] .. MEM[arg + WORD_SIZE - 1] in TMP
#define LOAD 0

// store TMP in MEM[arg] .. MEM[arg + WORD_SIZE - 1]
#define STORE 1

// TMP[0 .. WORD_SIZE-1] &= MEM[arg + (0 .. WORD_SIZE-1)]
#define AND 2

// TMP[0 .. WORD_SIZE-1] |= MEM[arg + (0 .. WORD_SIZE-1)]
#define OR 3

// invert all bits in TMP[0 .. WORD_SIZE-1]
#define NOT 4

// set IP = arg
#define JMP 5

// force above definitions to persist across preprocessor passes
#define PERSIST #include __FILE__
PERSIST
""")
output.close()

# make init_template.h
output = open("init_template.h", "w")
output.write("""/*
  This file defines the initial state of the VM.

  Programs are written by setting values in "main memory" to
  instruction codes and their arguments.
*/

// programs start at address 0
#define IP 0
#define AP 1

// initialize temp register
""")
for i in range(WORD_SIZE):
    output.write("#define TMP_%d 0\n" % i)
output.write("\n// initialize main memory\n")
for i in range(MEM_SIZE * WORD_SIZE):
    output.write("#define MEM_%d 0\n" % i)
output.close()

# make vm.h
output = open("vm.h", "w")
output.write("""/*
  The virtual machine has four components:
    Instruction pointer: variable IP
    Argument pointer: variable AP (= IP + 1)
    Temp register: variables TMP_ ## 0 .. TMP_ ## (WORD_SIZE - 1)
    Main memory: variables MEM_ ## 0 .. MEM_ ## (MEM_SIZE * WORD_SIZE - 1)

  Layout in memory:
    LO [instruction1 arg1 instruction2 arg2 .. instructionN argN] HI

  This file defines preprocessor logic that reads the current
  instruction from MEM[IP], evaluates it, increments IP, and
  sends the new VM state (as well as this code) onto the next
  preprocessor pass.

  In order for data to persist across preprocessor passes, each
  variable has a lowercase analogue. The state will alternate
  between uppercase and lowercase variables with each pass.

  Instructions are processed only when variables are in uppercase.
  This allows for time between instructions for changing values
  and referencing parts of memory. It also makes the code for handling
  each instruction consistent--uppercase always represents a value
  at the current time step, and lowercase always represents a value
  at the next time step.
*/
""")

# if f(upcase) is a function that emits code involving uppercase
# identifiers if upcase = True and lowercase if upcase = False,
# emit code that automatically handles both cases
def generate(f):
    output.write("#ifdef IP\n")
    f(True)
    output.write("#else\n")
    f(False)
    output.write("#endif\n")

output.write("""
// To allow data to persist across passes, we need to emit
// directives but not evaluate them.
#define DEFINE #define
#define IF #if
#define ELSE #else
#define ELIF #elif
#define ENDIF #endif
""")

# emit code to allow identifier to persist across passes
def persist(identifier, upcase):
    lo = identifier.lower()
    up = identifier.upper()
    if upcase:
        output.write("DEFINE %s %s\n" % (lo, up))
    else:
        output.write("DEFINE %s %s\n" % (up, lo))

# emit code to allow entire state to persist, assuming the current pass
# uses uppercase variables if upcase = true
def persist_all(upcase):
    if not upcase:
        persist("IP", upcase)
        persist("AP", upcase)
    for i in range(WORD_SIZE):
        persist("TMP_%d" % i, upcase)
    for i in range(MEM_SIZE * WORD_SIZE):
        persist("MEM_%d" % i, upcase)

output.write("\n// Allow state to persist across passes\n")
generate(persist_all)

# emit code to increment ip
output.write("\n// increment IP and AP\n")
def inc_ptrs(upcase):
    if not upcase:
        return
    elses = 0
    n = MEM_SIZE * WORD_SIZE
    for i in range(n):
        directive = "#if" if i == 0 else "#elif"
        output.write("%s IP == %d\n" % (directive, i))

        # add 2 to pointers to skip over the argument of past instr
        output.write("DEFINE ip %d\n" % ((i + 2) % n))
        output.write("DEFINE ap %d\n" % ((i + 3) % n))
    output.write("#endif\n")

generate(inc_ptrs)

output.write("""
// Needed to reference data in memory
#define CAT(a, b) a ## b
#define EQUALS(i, j) IF CAT(MEM_, i) == j
#define EEQUALS(i, j) ELIF CAT(MEM_, i) == j
#define GET(i, j) DEFINE CAT(tmp_, i) CAT(MEM_, j)
#define SET(i, j) DEFINE CAT(mem_, i) CAT(TMP_, j)
#define HIGH(i) DEFINE CAT(tmp_, i) 1
#define LOW(i) DEFINE CAT(tmp_, i) 0
#define BOTHTRUE(i, j) IF (CAT(MEM_, i) != 0) && (CAT(TMP_, j) != 0)
#define BOTHFALSE(i, j) IF (CAT(MEM_, i) == 0) && (CAT(TMP_, j) == 0)
#define TMPTRUE(i) IF CAT(TMP_, i) != 0
#define MEMTRUE(i) IF CAT(MEM_, i) != 0
#define SETPTR(p, i) DEFINE p CAT(MEM_, i)
""")

# given a dictionary { instruction_name: handler(upcase) }, emit
# code that executes instructions based on the value of MEM[IP]
# for the corresponding value of upcase
def step(handlers, upcase):
    if not upcase:
        return
    directive = "EQUALS"
    for instruction in handlers:
        handler = handlers[instruction]
        output.write("%s(IP, %s)\n" % (directive, instruction))
        handler()
        directive = "EEQUALS"
    output.write("ENDIF\n")

# generic handler for a binary operator
# f(i, j) is a function that emits code to perform the relevant
# operation for memory location i and bit index j
def handle_generic_op(f):
    directive = "EQUALS"
    for i in range(MEM_SIZE):
        output.write("%s(AP, %d)\n" % (directive, i))
        for j in range(WORD_SIZE):
            f(i, j)
        directive = "EEQUALS"
    output.write("ENDIF\n")    

# handle the LOAD instruction
def handle_load():
    def f(i, j):
        output.write("GET(%d, %d)\n" % (j, i * WORD_SIZE + j))
    handle_generic_op(f)

# handle the STORE instruction
def handle_store():
    def f(i, j):
        output.write("SET(%d, %d)\n" % (i * WORD_SIZE + j, j))
    handle_generic_op(f)

# handle the AND instruction
def handle_and():
    def f(i, j):
        output.write("BOTHTRUE(%d, %d)\n" % (i * WORD_SIZE + j, j))
        output.write("HIGH(%d)\n" % j)
        output.write("ELSE\n")
        output.write("LOW(%d)\n" % j)
        output.write("ENDIF\n")
    handle_generic_op(f)

# handle the OR instruction
def handle_or():
    def f(i, j):
        output.write("BOTHFALSE(%d, %d)\n" % (i * WORD_SIZE + j, j))
        output.write("LOW(%d)\n" % j)
        output.write("ELSE\n")
        output.write("HIGH(%d)\n" % j)
        output.write("ENDIF\n")
    handle_generic_op(f)

def handle_not():
    for j in range(WORD_SIZE):
        output.write("TMPTRUE(%d)\n" % j)
        output.write("LOW(%d)\n" % j)
        output.write("ELSE\n")
        output.write("HIGH(%d)\n" % j)
        output.write("ENDIF\n")

def handle_jmp():
    directive = "EQUALS"
    output.write("SETPTR(ip, AP)\n")
    n = MEM_SIZE * WORD_SIZE
    for i in range(n):
        output.write("%s(AP, %d)\n" % (directive, i))
        output.write("DEFINE ap %d\n" % ((i + 1) % n))
        directive = "EEQUALS"
    output.write("ENDIF\n")    

# wrap all generating code into a nice function for generate()
def curried_step(upcase):
    handlers = {
        "LOAD": handle_load,
        "STORE": handle_store,
        "AND": handle_and,
        "OR": handle_or,
        "NOT": handle_not,
        "JMP": handle_jmp
    }
    return step(handlers, upcase)

output.write("\n// Process the instruction at MEM[IP]\n")
generate(curried_step)

output.write("""
/*
  Finally, all of the above declarations need to persist across passes.
*/
#define PERSIST #include __FILE__
PERSIST
""")
output.close()
