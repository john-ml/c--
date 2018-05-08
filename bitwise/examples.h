#define __PREFIX__ true
#define __DEFINE__ #define
#define NOP(...) (__VA_ARGS__)
#define DEF(key, val) __DEFINE__ __PREFIX__ ## key val
#define GET(key) __PREFIX__ ## key

#define persist #include __FILE__
persist
