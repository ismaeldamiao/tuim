# Interfaces fo Tuim

## .

1. Open the executable to read
2. Parse the executable and load the program image in the memory
3. Last tow steps are repeated for all shared object dependencies
4. Relocate the code for the program and all dependencies
5. Execute Initialization Functions
6. If neccessary, prepare the machine state
7. Execute the program
8. If the program return, execute Termination Functions.

## Usage

```
tuim run [-m machine] <file> [args ...]
```

## Building

On POSIX systems you only need to build the `src/main.c` file using a
C compiler. For example:

```sh
clang -o bin/tuim -D _POSIX_C_SOURCE=200809L src/main.c
```

## Donations
