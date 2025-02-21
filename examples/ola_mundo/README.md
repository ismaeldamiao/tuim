# Olá Mundo!

This program prints "Ola Mundo!" on stadard output.
The main program is implemented in C,
it call the function `puts`.
`puts` is implemented using the `__tuim_fpuc` system call.

Features:

* The program is self contained, i.e., have no external dependencies.
* Test executable loading.
* Test shared object loading.
* Test realocations.
* Test a system call.
* Test the command line tools for developers.
* Test the CMake build.

## Building

In order to build you need `ld.lld` and `clang` commands
and need to setup the environment:

```sh
. "${TUIM_HOME}/share/env-host.sh"
mkdir tmp
mkdir lib
mkdir bin
```

You can the following command to build program;

```sh
cc -E src/puts.S | as -o tmp/puts.o -
cc -o tmp/main.o src/main.c
ld --shared -o lib/libputs.so tmp/puts.o
ld -e main -o bin/main.elf -L lib/ -l puts tmp/main.o
```

## Executing

Since the executable is requesting a custom shared object
the environment must be prepared before execution.

```sh
LD_LIBRARY_PATH=./lib: tuim bin/main.elf
```
