/*
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
#define QUERY(i) IF CAT(MEM_, i) == 
#define EQUERY(i) ELIF CAT(MEM_, i) ==
#define GET(i, j) DEFINE CAT(tmp_, i) CAT(MEM_, j)
#define SET(i, j) DEFINE CAT(mem_, i) CAT(TMP_, j)

// Process the instruction at MEM[IP]
#ifdef IP
QUERY(IP) LOAD
QUERY(AP) 0
GET(0, 0)
GET(1, 1)
GET(2, 2)
GET(3, 3)
EQUERY(AP) 1
GET(0, 4)
GET(1, 5)
GET(2, 6)
GET(3, 7)
EQUERY(AP) 2
GET(0, 8)
GET(1, 9)
GET(2, 10)
GET(3, 11)
ENDIF
EQUERY(IP) STORE
QUERY(AP) 0
SET(0, 0)
SET(1, 1)
SET(2, 2)
SET(3, 3)
EQUERY(AP) 1
SET(4, 0)
SET(5, 1)
SET(6, 2)
SET(7, 3)
EQUERY(AP) 2
SET(8, 0)
SET(9, 1)
SET(10, 2)
SET(11, 3)
ENDIF
ENDIF
#else
#endif

/*
  Finally, all of the above declarations need to persist across passes.
*/
#define PERSIST #include __FILE__
PERSIST
