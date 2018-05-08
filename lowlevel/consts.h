/*
  Instructions and their arguments are encoded as positive integers.
*/

// store MEM[arg] .. MEM[arg + WORD_SIZE - 1] in TMP
#define LOAD 0

// store TMP in MEM[arg] .. MEM[arg + WORD_SIZE - 1]
#define STORE 1

// force above definitions to persist across preprocessor passes
#define PERSIST #include __FILE__
PERSIST
