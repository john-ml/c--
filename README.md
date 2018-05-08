# C--

`c--` is a bash script that repeatedly runs the C/C++ preprocessor `cpp` on a given
input file until the output stops changing.

Usage: `c-- <filename> --verbosity=n`

Verbosity levels:
* `--verbosity=0`: only output the final result
* `--verbosity=1`: also output the result of intermediate passes
* `--verbosity=2`: also output any errors/warnings emitted by `cpp`

## Tuple appproach

[prelude.h](https://github.com/johnli0135/c--/blob/master/tuple/prelude.h) uses parameter lists
as tuples to represent data and parametrized macros to represent functions.
(`f(g(x))` is written `f g x`.)

Example:
