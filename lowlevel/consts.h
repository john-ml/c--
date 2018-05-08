/*
  The virtual machine has four components:
    Instruction pointer: variable IP
    Argument pointer: variable AP (= IP + 1)
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
