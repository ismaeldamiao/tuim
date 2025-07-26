# C Programing Language

[I. F. F. dos Santos](mailto:ismaellxd@gmail.com)

Last modified: July 23, 2025.

!> Tuim is a project in active development,
for now it is designed only for test purposes.

## Abstract

TODO: Write this section.

## Introduction

TODO: Write this section.

## Building C application

### Compiling

The following environment variables shall be used
within the Tuim's build system:
- `CC`: The name/path of a C compiler executable.
- `CFLAGS`: The used to configure the compilation.

For the purposes of this document these environment variables
shall be abble to use the following alias
to implement the command line tool `cc`
(see appendex "cc — compile standard C programs").

```sh
alias cc="${CC} ${CFLAGS}"
```

### Linking within the C Standard Library

The shared object holding external definitions from the C Standard Library
shall be named `libc.so`
and the linker flags to link with it shall be `-l c`.
The shared object `libc.so`
shall define no symbol except for symbols identical to
reserved identifiers for a hosted environment.

## Language ABI

TODO: Write this section.

## Standard Library ABI

TODO: Write this section.

Since Tuim is a freestanding environment the program entry point
is not the same as the C entry point,
therefore `libc.so` shall provide a routine named
`__libc_main` to wrap to the C entry point when the program initial state
is prepared by `tuim_exec`.
For example, a execuble compiled with
`-e program` and `-l c` linker flags
from (something like) the following source code
shall behaves as a standard C program when executed by
`tuim` utility.

```c
int main(int argc, char **argv)
{
   /* ... */
}

extern void __libc_main(typeof(&main));
void program(void){ __libc_main(main); }
```

## Conclusion

TODO: Write this section.

## References

TODO: Write this section.

## Appendex

### cc — compile standard C programs

TODO: Write this appendex.
