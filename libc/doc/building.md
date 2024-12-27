# Building the C Standard Library

1. Use a freestanding compiler to build the object code:

   ```C
   cc -D <MACRO> -nostdinc -I src/include -o .build/libc.o src/libc.c
   ```

   Where `<MACRO>` must be replaced by one of:
   * `__linux__` for linux based OS.
   * `_Win32` for Windows.
1. Use the linker to build the shared object:

   ```C
   ld --shared -o .build/libc.so .build/libc.o
   ```

   But note that if you building for ARM you need the
   ARM Embeded Application Binary Interface instaled and link with it as

   ```C
   ld --shared -o .build/libc.so -laeabi .build/libc.o
   ```

## Installing

To install the library you need to copy the file `libc.so`
to the default library path and `include` dource directory to the
default include directory.

For example:

```C
cp src/include $PREFIX/lib/tuim/include
cp .build/libc.so $PREFIX/lib/tuim/lib
```
