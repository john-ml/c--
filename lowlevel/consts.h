/*
  Instructions and their arguments are encoded as positive integers.
*/

// store MEM[arg] .. MEM[arg + WORD_SIZE - 1] in TMP
#define LOAD 0000
#define LOAD_0 0
#define LOAD_1 0
#define LOAD_2 0
#define LOAD_3 0

// store arg in TMP
#define LOADIMM 0001
#define LOADIMM_0 0
#define LOADIMM_1 0
#define LOADIMM_2 0
#define LOADIMM_3 1

// store TMP in MEM[arg] .. MEM[arg + WORD_SIZE - 1]
#define STORE 0010
#define STORE_0 0
#define STORE_1 0
#define STORE_2 1
#define STORE_3 0

// TMP[0 .. WORD_SIZE-1] &= MEM[arg + (0 .. WORD_SIZE-1)]
#define AND 0011
#define AND_0 0
#define AND_1 0
#define AND_2 1
#define AND_3 1

// TMP[0 .. WORD_SIZE-1] |= MEM[arg + (0 .. WORD_SIZE-1)]
#define OR 0100
#define OR_0 0
#define OR_1 1
#define OR_2 0
#define OR_3 0

// invert all bits in TMP[0 .. WORD_SIZE-1]
#define NOT 0101
#define NOT_0 0
#define NOT_1 1
#define NOT_2 0
#define NOT_3 1

// set IP = arg
#define JMP 0110
#define JMP_0 0
#define JMP_1 1
#define JMP_2 1
#define JMP_3 0

// force above definitions to persist across preprocessor passes
#define PERSIST #include __FILE__
PERSIST
