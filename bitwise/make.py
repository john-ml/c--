# This script generates consts.h, init_template.h, vm.h 

MEM_SIZE = 1
WORD_SIZE = 4

# make consts.h
output = open("consts.h", "w")
output.write("""/*
  The virtual machine has three components:
    Instruction pointer: variable IP
    Temp register: variables TMP_ ## 0 .. TMP_ ## (WORD_SIZE - 1)
    Main memory: variables MEM_ ## 0 .. MEM_ ## (MEM_SIZE * WORD_SIZE - 1)

  Instructions and their arguments are encoded as positive integers.

  Layout in memory:
    LO [instruction1 arg1 instruction2 arg2 .. instructionN argN] HI
*/

// store MEM[arg] .. MEM[arg + WORD_SIZE - 1] in TMP
#define LOAD 0

// store TMP in MEM[arg] .. MEM[arg + WORD_SIZE - 1]
#define STORE 1

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
  This file defines preprocessor logic that reads the current
  instruction from MEM[IP], evaluates it, increments IP, and
  sends the new VM state (as well as this code) onto the next
  preprocessor pass.

  In order for data to persist across preprocessor passes, each
  variable has a lowercase analogue. The state will alternate
  between uppercase and lowercase variables with each pass.
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
/*
  To allow data to persist across passes, we need to emit
  #define directives but not evaluate them.
*/

// Defer evaluation of a #define statement
#define DEFINE #define
""")

output.write("\n// increment IP\n")

# emit code to increment ip
def inc_ip(upcase):
    ip = "IP" if upcase else "ip"
    next_ip = "ip" if upcase else "IP"
    elses = 0
    n = MEM_SIZE * WORD_SIZE
    for i in range(n):
        directive = "#if" if i == 0 else "#elif"
        output.write("%s %s == %d\n" % (directive, ip, i))
        output.write("DEFINE %s %s\n" % (next_ip, (i + 1) % n))
    output.write("#endif\n")

generate(inc_ip)

# emit code to emit code to allow identifier to persist across passes
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
    for i in range(WORD_SIZE):
        persist("TMP_%d" % i, upcase)
    for i in range(MEM_SIZE * WORD_SIZE):
        persist("MEM_%d" % i, upcase)

output.write("\n// Allow state to persist across passes\n")
generate(persist_all)

output.write("""
/*
  Finally, all of the above declarations need to persist across passes.
*/
#define PERSIST #include __FILE__
PERSIST
""")
output.close()
