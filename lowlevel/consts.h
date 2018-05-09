/*
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
