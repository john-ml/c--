/*
  This file defines preprocessor logic that reads the current
  instruction from MEM[IP], evaluates it, increments IP, and
  sends the new VM state (as well as this code) onto the next
  preprocessor pass.

  In order for data to persist across preprocessor passes, each
  variable has a lowercase analogue. The state will alternate
  between uppercase and lowercase variables with each pass.
*/

/*
  To allow data to persist across passes, we need to emit
  #define directives but not evaluate them.
*/

// Defer evaluation of a #define statement
#define DEFINE #define

// increment IP
#ifdef IP
#if IP == 0
DEFINE ip 1
#elif IP == 1
DEFINE ip 2
#elif IP == 2
DEFINE ip 3
#elif IP == 3
DEFINE ip 0
#endif
#else
#if ip == 0
DEFINE IP 1
#elif ip == 1
DEFINE IP 2
#elif ip == 2
DEFINE IP 3
#elif ip == 3
DEFINE IP 0
#endif
#endif

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
#else
DEFINE TMP_0 tmp_0
DEFINE TMP_1 tmp_1
DEFINE TMP_2 tmp_2
DEFINE TMP_3 tmp_3
DEFINE MEM_0 mem_0
DEFINE MEM_1 mem_1
DEFINE MEM_2 mem_2
DEFINE MEM_3 mem_3
#endif

/*
  Finally, all of the above declarations need to persist across passes.
*/
#define PERSIST #include __FILE__
PERSIST
