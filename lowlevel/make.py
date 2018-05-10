# This script generates consts.h, init_template.h, vm.h 

MEM_SIZE = 5
WORD_SIZE = 4

# convert an integer i into a zero-padded binary representation
def binarize(i, word_size=WORD_SIZE):
    return "{0:b}".format(i).zfill(word_size)

# write output of size n given by f(i, b) where i is an integer
# and b is its binarized representation
def write_block(n, f):
    for i in range(n):
        b = binarize(i)
        f(i, b)

# write a block over words
def write_word_block(f):
    write_block(WORD_SIZE, f)

# write a block over memory
def write_mem_block(f):
    write_block(WORD_SIZE * MEM_SIZE, f)


# make consts.h
output = open("consts.h", "w")
output.write("""/*
  Instructions and their arguments are encoded as positive integers.
*/
""")

# define an instruction given a name 
def instruction(name):
    binarized = binarize(instruction.opcode)
    output.write("#define %s %s\n" % (name, binarized))
    write_word_block(lambda i, b:
        output.write("#define %s_%d %s\n" % (name, i, binarized[i]))
    )
    instruction.opcode += 1
instruction.opcode = 0

output.write("\n// store MEM[arg] .. MEM[arg + WORD_SIZE - 1] in TMP\n")
instruction("LOAD")

output.write("\n// store arg in TMP\n")
instruction("LOADIMM")

output.write("\n// store TMP in MEM[arg] .. MEM[arg + WORD_SIZE - 1]\n")
instruction("STORE")

output.write("\n// TMP[0 .. WORD_SIZE-1] &= MEM[arg + (0 .. WORD_SIZE-1)]\n")
instruction("AND")

output.write("\n// TMP[0 .. WORD_SIZE-1] |= MEM[arg + (0 .. WORD_SIZE-1)]\n")
instruction("OR")

output.write("\n// invert all bits in TMP[0 .. WORD_SIZE-1]\n")
instruction("NOT")

output.write("\n// set IP = arg\n")
instruction("JMP")

output.write("""
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
""")

output.write("\n// programs start at address 0\n")
write_word_block(lambda i, b: output.write("#define IP_%s %s\n" % (b, b)))
write_word_block(lambda i, b: output.write("#define AP_%s %s\n" % (b, binarize(WORD_SIZE + i))))

output.write("\n// initialize temp register\n")
write_word_block(lambda i, b: output.write("#define TMP_%s 0\n" % b))

output.write("\n// initialize main memory\n")
write_mem_block(lambda i, b: output.write("#define MEM_%s 0\n" % b))
output.close()

# make vm.h
output = open("vm.h", "w")
output.write("""/*
  The virtual machine has four components:
    Instruction pointer: variables IP_ ## 0 .. IP_ ## (WORD_SIZE - 1)
    Argument pointer: variables AP_ ## 0 .. AP_ ## (WORD_SIZE - 1)
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
    output.write("#ifdef IP_%s\n" % ('0' * WORD_SIZE))
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
        write_word_block(lambda i, b: persist("IP_%s" % b, upcase))
        write_word_block(lambda i, b: persist("AP_%s" % b, upcase))
    write_word_block(lambda i, b: persist("TMP_%s" % b, upcase))
    write_mem_block(lambda i, b: persist("MEM_%s" % b, upcase))

output.write("\n// Allow state to persist across passes\n")
generate(persist_all)

# emit code to set ip and ap
def set_ptrs(next_ip):
    n = MEM_SIZE * WORD_SIZE
    next_ap = next_ip + WORD_SIZE
    write_word_block(lambda i, b: output.write("DEFINE ip_%s %s\n" % (
        b,
        binarize((next_ip + i) % n)
    )))
    write_word_block(lambda i, b: output.write("DEFINE ap_%s %s\n" % (
        b,
        binarize((next_ap + i) % n)
    )))

# emit code to increment ip
output.write("\n// increment IP and AP\n")
def inc_ptrs(upcase):
    if not upcase:
        return
    for i in range(MEM_SIZE):
        directive = "#if" if i == 0 else "#elif"
        output.write("%s IP_%s == %s\n" % (
            directive,
            '0' * WORD_SIZE,
            binarize(i * WORD_SIZE))
        )

        # add 2 to pointers to skip over the argument of past instr
        set_ptrs(i * WORD_SIZE + 2 * WORD_SIZE)
    output.write("#endif\n")

generate(inc_ptrs)

output.write("""
// Needed to reference data in memory
#define CAT(a, b) a ## b
#define EMPTY()
#define DEFER(x) x EMPTY()
#define EVAL(...) __VA_ARGS__
#define EQUALS(i, j) IF i == j
#define EEQUALS(i, j) ELIF i == j
#define GET(i, j) DEFINE CAT(tmp_, i) CAT(MEM_, j)
#define SETIMM(i, j) DEFINE CAT(tmp_, i) j
#define SET(i, j) DEFINE CAT(mem_, i) CAT(TMP_, j)
#define HIGH(i) DEFINE CAT(tmp_, i) 1
#define LOW(i) DEFINE CAT(tmp_, i) 0
#define BOTHTRUE(i, j) IF (CAT(MEM_, i) != 0) && (CAT(TMP_, j) != 0)
#define BOTHFALSE(i, j) IF (CAT(MEM_, i) == 0) && (CAT(TMP_, j) == 0)
#define TMPTRUE(i) IF CAT(TMP_, i) != 0
#define MEMTRUE(i) IF CAT(MEM_, i) != 0
#define SETPTR(p, i) DEFINE p CAT(MEM_, i)
""")

output.write("\n// Extract instruction and argument\n")

# generate code to extract ptr_type into variable var
def extract(ptr_type, var):
    bits = ["EVAL(DEFER(CAT)(MEM_, %sP_%s))" % (ptr_type, binarize(i)) for i in range(WORD_SIZE)]
    accu = ""
    for i in range(len(bits)):
        accu = "EVAL(DEFER(CAT)(%s, %s))" % (accu, bits[i])
    output.write("#define %s %s\n" % (var, accu))

# generate conditional statement that checks if var is equal to const,
# assuming both values are binarized integers
# uses conditional directive directive
def condexpr(directive, var, const):
    checkstr = "%s(EVAL(DEFER(CAT)(1, %s)), EVAL(DEFER(CAT)(1, %s)))\n"
    output.write(checkstr % (directive, var, const))
    return 

extract("I", "INSTR")
extract("A", "ARG")

# generic handler for a binary operator
# f(i, j, k, bi, bj, bk) is a function that emits code to perform the relevant
# operation for memory location i and bit index j
# k = i * WORD_SIZE + j
# with binarized equivalents bi, bj, bk
def handle_generic_op(f):
    directive = "EQUALS"
    for i in range(MEM_SIZE):
        condexpr(directive, "ARG", binarize(i))
        for j in range(WORD_SIZE):
            f(
                i,
                j,
                i * WORD_SIZE + j,
                binarize(i),
                binarize(j),
                binarize(i * WORD_SIZE + j)
            )
        directive = "EEQUALS"
    output.write("ENDIF\n")    

# handle the LOAD instruction
def handle_load():
    def f(i, j, k, bi, bj, bk):
        output.write("GET(%s, %s)\n" % (bj, bk))
    handle_generic_op(f)

# handle the LOADIMM instruction
def handle_loadimm():
    write_word_block(lambda i, b: output.write(
        "SETIMM(%s, EVAL(DEFER(CAT)(MEM_, AP_%s)))\n" % (b, b)
    ))

# handle the STORE instruction
def handle_store():
    def f(i, j, k, bi, bj, bk):
        output.write("SET(%s, %s)\n" % (bk, bj))
    handle_generic_op(f)

# handle the AND instruction
def handle_and():
    def f(i, j, k, bi, bj, bk):
        output.write("BOTHTRUE(%s, %s)\n" % (bk, bj))
        output.write("HIGH(%s)\n" % bj)
        output.write("ELSE\n")
        output.write("LOW(%s)\n" % bj)
        output.write("ENDIF\n")
    handle_generic_op(f)

# handle the OR instruction
def handle_or():
    def f(i, j, k, bi, bj, bk):
        output.write("BOTHFALSE(%s, %s)\n" % (bk, bj))
        output.write("LOW(%s)\n" % bj)
        output.write("ELSE\n")
        output.write("HIGH(%s)\n" % bj)
        output.write("ENDIF\n")
    handle_generic_op(f)

# handle the NOT instruction
def handle_not():
    def write_one(i, b):
        output.write("TMPTRUE(%s)\n" % b)
        output.write("LOW(%s)\n" % b)
        output.write("ELSE\n")
        output.write("HIGH(%s)\n" % b)
        output.write("ENDIF\n")
    write_word_block(write_one)

# handle the JMP instruction
def handle_jmp():
    directive = "EQUALS"
    n = MEM_SIZE * WORD_SIZE
    for i in range(n):
        condexpr(directive, "ARG", binarize(i))
        set_ptrs(i)
        directive = "EEQUALS"
    output.write("ENDIF\n")    

# given a dictionary { instruction_name: handler(upcase) }, emit
# code that executes instructions based on the value of MEM[IP]
# for the corresponding value of upcase
def step(handlers, upcase):
    if not upcase:
        return
    directive = "EQUALS"
    for instruction in handlers:
        handler = handlers[instruction]
        condexpr(directive, "INSTR", instruction)
        handler()
        directive = "EEQUALS"
    output.write("ENDIF\n")

# wrap all generating code into a nice function for generate()
def curried_step(upcase):
    handlers = {
        "LOAD": handle_load,
        "LOADIMM": handle_loadimm,
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

if __name__ == "__main__":
    print("hi")
