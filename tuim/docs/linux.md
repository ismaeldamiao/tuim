# Tuim's Dynamic Loader and Linker - Linux build

Choose the compiler and linker, for example:

```sh
export CC="clang"
export CFLAGS="-Wall -Wextra -Wpedantic -O3 -c"
export LD="clang"
export LDFLAGS="-fuse-ld=lld"
```

Then you can build with:

```sh
make libtuim.a
```

This shall generate the `lib/libtuim.a` file.
