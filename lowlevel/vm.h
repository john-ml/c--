/*
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

// To allow data to persist across passes, we need to emit
// directives but not evaluate them.
#define DEFINE #define
#define IF #if
#define ELSE #else
#define ELIF #elif
#define ENDIF #endif

// Allow state to persist across passes
#ifdef IP
DEFINE tmp_0 TMP_0
DEFINE tmp_1 TMP_1
DEFINE tmp_2 TMP_2
DEFINE tmp_3 TMP_3
DEFINE mem_0 MEM_0
DEFINE mem_1 MEM_1
DEFINE mem_2 MEM_2
DEFINE mem_3 MEM_3
DEFINE mem_4 MEM_4
DEFINE mem_5 MEM_5
DEFINE mem_6 MEM_6
DEFINE mem_7 MEM_7
DEFINE mem_8 MEM_8
DEFINE mem_9 MEM_9
DEFINE mem_10 MEM_10
DEFINE mem_11 MEM_11
#else
DEFINE IP ip
DEFINE AP ap
DEFINE TMP_0 tmp_0
DEFINE TMP_1 tmp_1
DEFINE TMP_2 tmp_2
DEFINE TMP_3 tmp_3
DEFINE MEM_0 mem_0
DEFINE MEM_1 mem_1
DEFINE MEM_2 mem_2
DEFINE MEM_3 mem_3
DEFINE MEM_4 mem_4
DEFINE MEM_5 mem_5
DEFINE MEM_6 mem_6
DEFINE MEM_7 mem_7
DEFINE MEM_8 mem_8
DEFINE MEM_9 mem_9
DEFINE MEM_10 mem_10
DEFINE MEM_11 mem_11
#endif

// increment IP and AP
#ifdef IP
#if IP == 0
DEFINE ip 2
DEFINE ap 3
#elif IP == 1
DEFINE ip 3
DEFINE ap 4
#elif IP == 2
DEFINE ip 4
DEFINE ap 5
#elif IP == 3
DEFINE ip 5
DEFINE ap 6
#elif IP == 4
DEFINE ip 6
DEFINE ap 7
#elif IP == 5
DEFINE ip 7
DEFINE ap 8
#elif IP == 6
DEFINE ip 8
DEFINE ap 9
#elif IP == 7
DEFINE ip 9
DEFINE ap 10
#elif IP == 8
DEFINE ip 10
DEFINE ap 11
#elif IP == 9
DEFINE ip 11
DEFINE ap 0
#elif IP == 10
DEFINE ip 0
DEFINE ap 1
#elif IP == 11
DEFINE ip 1
DEFINE ap 2
#endif
#else
#endif

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

// Process the instruction at MEM[IP]
#ifdef IP
EQUALS(IP, LOAD)
EQUALS(AP, 0)
GET(0, 0)
GET(1, 1)
GET(2, 2)
GET(3, 3)
EEQUALS(AP, 1)
GET(0, 4)
GET(1, 5)
GET(2, 6)
GET(3, 7)
EEQUALS(AP, 2)
GET(0, 8)
GET(1, 9)
GET(2, 10)
GET(3, 11)
ENDIF
EEQUALS(IP, STORE)
EQUALS(AP, 0)
SET(0, 0)
SET(1, 1)
SET(2, 2)
SET(3, 3)
EEQUALS(AP, 1)
SET(4, 0)
SET(5, 1)
SET(6, 2)
SET(7, 3)
EEQUALS(AP, 2)
SET(8, 0)
SET(9, 1)
SET(10, 2)
SET(11, 3)
ENDIF
EEQUALS(IP, AND)
EQUALS(AP, 0)
BOTHTRUE(0, 0)
HIGH(0)
ELSE
LOW(0)
ENDIF
BOTHTRUE(1, 1)
HIGH(1)
ELSE
LOW(1)
ENDIF
BOTHTRUE(2, 2)
HIGH(2)
ELSE
LOW(2)
ENDIF
BOTHTRUE(3, 3)
HIGH(3)
ELSE
LOW(3)
ENDIF
EEQUALS(AP, 1)
BOTHTRUE(4, 0)
HIGH(0)
ELSE
LOW(0)
ENDIF
BOTHTRUE(5, 1)
HIGH(1)
ELSE
LOW(1)
ENDIF
BOTHTRUE(6, 2)
HIGH(2)
ELSE
LOW(2)
ENDIF
BOTHTRUE(7, 3)
HIGH(3)
ELSE
LOW(3)
ENDIF
EEQUALS(AP, 2)
BOTHTRUE(8, 0)
HIGH(0)
ELSE
LOW(0)
ENDIF
BOTHTRUE(9, 1)
HIGH(1)
ELSE
LOW(1)
ENDIF
BOTHTRUE(10, 2)
HIGH(2)
ELSE
LOW(2)
ENDIF
BOTHTRUE(11, 3)
HIGH(3)
ELSE
LOW(3)
ENDIF
ENDIF
EEQUALS(IP, OR)
EQUALS(AP, 0)
BOTHFALSE(0, 0)
LOW(0)
ELSE
HIGH(0)
ENDIF
BOTHFALSE(1, 1)
LOW(1)
ELSE
HIGH(1)
ENDIF
BOTHFALSE(2, 2)
LOW(2)
ELSE
HIGH(2)
ENDIF
BOTHFALSE(3, 3)
LOW(3)
ELSE
HIGH(3)
ENDIF
EEQUALS(AP, 1)
BOTHFALSE(4, 0)
LOW(0)
ELSE
HIGH(0)
ENDIF
BOTHFALSE(5, 1)
LOW(1)
ELSE
HIGH(1)
ENDIF
BOTHFALSE(6, 2)
LOW(2)
ELSE
HIGH(2)
ENDIF
BOTHFALSE(7, 3)
LOW(3)
ELSE
HIGH(3)
ENDIF
EEQUALS(AP, 2)
BOTHFALSE(8, 0)
LOW(0)
ELSE
HIGH(0)
ENDIF
BOTHFALSE(9, 1)
LOW(1)
ELSE
HIGH(1)
ENDIF
BOTHFALSE(10, 2)
LOW(2)
ELSE
HIGH(2)
ENDIF
BOTHFALSE(11, 3)
LOW(3)
ELSE
HIGH(3)
ENDIF
ENDIF
EEQUALS(IP, NOT)
TMPTRUE(0)
LOW(0)
ELSE
HIGH(0)
ENDIF
TMPTRUE(1)
LOW(1)
ELSE
HIGH(1)
ENDIF
TMPTRUE(2)
LOW(2)
ELSE
HIGH(2)
ENDIF
TMPTRUE(3)
LOW(3)
ELSE
HIGH(3)
ENDIF
EEQUALS(IP, JMP)
SETPTR(ip, AP)
EQUALS(AP, 0)
DEFINE ap 1
EEQUALS(AP, 1)
DEFINE ap 2
EEQUALS(AP, 2)
DEFINE ap 3
EEQUALS(AP, 3)
DEFINE ap 4
EEQUALS(AP, 4)
DEFINE ap 5
EEQUALS(AP, 5)
DEFINE ap 6
EEQUALS(AP, 6)
DEFINE ap 7
EEQUALS(AP, 7)
DEFINE ap 8
EEQUALS(AP, 8)
DEFINE ap 9
EEQUALS(AP, 9)
DEFINE ap 10
EEQUALS(AP, 10)
DEFINE ap 11
EEQUALS(AP, 11)
DEFINE ap 0
ENDIF
ENDIF
#else
#endif

/*
  Finally, all of the above declarations need to persist across passes.
*/
#define PERSIST #include __FILE__
PERSIST
