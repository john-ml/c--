/*
  Abbreviations of commonly used patterns.
*/

// standard directives 
#define __IF__ #if
#define __IFDEF__ #ifdef
#define __IFNDEF__ #ifndef
#define __ELIF__ #elif
#define __ELSE__ #else
#define __END__ #endif
#define __DEFINE__ #define
#define __UNDEF__ #undef
#define __ERROR__ #error
#define __INCLUDE__ #include
#define IF() __IF__ 1 == 
#define IFNOT() __IF__ 1 != 
#define IFDEF() __IFDEF__
#define IFNDEF() __IFNDEF__
#define ELIF() __ELIF__
#define ELSE() __ELSE__
#define END() __END__
#define DEFINE() __DEFINE__
#define UNDEF() __UNDEF__
#define ERROR() __ERROR__
#define INCLUDE() __INCLUDE__

// setting values
#define ON(x) DEFINE x() 1
#define OFF(x) DEFINE x() 0

// get ith bit of x
#define BIT(i, x) x ## _ ## i

#define PERSIST #include __FILE__
PERSIST

