/*
  Instructions and their arguments are encoded as positive integers.
*/

// store MEM[arg] .. MEM[arg + WORD_SIZE - 1] in TMP
#define LOAD 00000000
#define LOAD_0 0
#define LOAD_1 0
#define LOAD_2 0
#define LOAD_3 0
#define LOAD_4 0
#define LOAD_5 0
#define LOAD_6 0
#define LOAD_7 0

// store arg in TMP
#define LOADIMM 00000001
#define LOADIMM_0 0
#define LOADIMM_1 0
#define LOADIMM_2 0
#define LOADIMM_3 0
#define LOADIMM_4 0
#define LOADIMM_5 0
#define LOADIMM_6 0
#define LOADIMM_7 1

// store TMP in MEM[arg] .. MEM[arg + WORD_SIZE - 1]
#define STORE 00000010
#define STORE_0 0
#define STORE_1 0
#define STORE_2 0
#define STORE_3 0
#define STORE_4 0
#define STORE_5 0
#define STORE_6 1
#define STORE_7 0

// TMP[0 .. WORD_SIZE-1] &= MEM[arg + (0 .. WORD_SIZE-1)]
#define AND 00000011
#define AND_0 0
#define AND_1 0
#define AND_2 0
#define AND_3 0
#define AND_4 0
#define AND_5 0
#define AND_6 1
#define AND_7 1

// TMP[0 .. WORD_SIZE-1] |= MEM[arg + (0 .. WORD_SIZE-1)]
#define OR 00000100
#define OR_0 0
#define OR_1 0
#define OR_2 0
#define OR_3 0
#define OR_4 0
#define OR_5 1
#define OR_6 0
#define OR_7 0

// invert all bits in TMP[0 .. WORD_SIZE-1]
#define NOT 00000101
#define NOT_0 0
#define NOT_1 0
#define NOT_2 0
#define NOT_3 0
#define NOT_4 0
#define NOT_5 1
#define NOT_6 0
#define NOT_7 1

// set IP = arg
#define JMP 00000110
#define JMP_0 0
#define JMP_1 0
#define JMP_2 0
#define JMP_3 0
#define JMP_4 0
#define JMP_5 1
#define JMP_6 1
#define JMP_7 0

// force above definitions to persist across preprocessor passes
#define PERSIST #include __FILE__
PERSIST
