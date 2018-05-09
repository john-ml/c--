/*
  Instructions and their arguments are encoded as positive integers.
*/

// store MEM[arg] .. MEM[arg + WORD_SIZE - 1] in TMP
#define LOAD 0000
#define LOAD_0 0
#define LOAD_1 0
#define LOAD_2 0
#define LOAD_3 0

// store TMP in MEM[arg] .. MEM[arg + WORD_SIZE - 1]
#define STORE 0001
#define STORE_0 0
#define STORE_1 0
#define STORE_2 0
#define STORE_3 1

// TMP[0 .. WORD_SIZE-1] &= MEM[arg + (0 .. WORD_SIZE-1)]
#define AND 0010
#define AND_0 0
#define AND_1 0
#define AND_2 1
#define AND_3 0

// TMP[0 .. WORD_SIZE-1] |= MEM[arg + (0 .. WORD_SIZE-1)]
#define OR 0011
#define OR_0 0
#define OR_1 0
#define OR_2 1
#define OR_3 1

// invert all bits in TMP[0 .. WORD_SIZE-1]
#define NOT 0100
#define NOT_0 0
#define NOT_1 1
#define NOT_2 0
#define NOT_3 0

// set IP = arg
#define JMP 0101
#define JMP_0 0
#define JMP_1 1
#define JMP_2 0
#define JMP_3 1

// force above definitions to persist across preprocessor passes
#define PERSIST #include __FILE__
PERSIST
