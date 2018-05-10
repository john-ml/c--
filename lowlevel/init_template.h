/*
  This file defines the initial state of the VM.

  Programs are written by setting values in "main memory" to
  instruction codes and their arguments.
*/

// programs start at address 0
#define IP_0000 0000
#define IP_0001 0001
#define IP_0010 0010
#define IP_0011 0011
#define AP_0000 0100
#define AP_0001 0101
#define AP_0010 0110
#define AP_0011 0111

// initialize temp register
#define TMP_0000 0
#define TMP_0001 0
#define TMP_0010 0
#define TMP_0011 0

// initialize main memory
#define MEM_0000 0
#define MEM_0001 0
#define MEM_0010 0
#define MEM_0011 0
#define MEM_0100 0
#define MEM_0101 0
#define MEM_0110 0
#define MEM_0111 0
#define MEM_1000 0
#define MEM_1001 0
#define MEM_1010 0
#define MEM_1011 0
#define MEM_1100 0
#define MEM_1101 0
#define MEM_1110 0
#define MEM_1111 0
#define MEM_10000 0
#define MEM_10001 0
#define MEM_10010 0
#define MEM_10011 0
