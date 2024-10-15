<h1 align="center">Command Line Interface</h1>

> [!CAUTION]
> This project is in beta stage, it may not work as espected
> and it's API may change at any time.

Command line interface for the Tuim's ELF loader,
to execute your programs from the command line.

## Usage

Given a executable called `app.elf` you can execute it with the command.

```bash
tuim app.elf
```

Some technical requirements, for developers, are:

- The executable must be a ELF file.
- The ELF must be a shared object compiled as position independent code.
- Dependency libraries (shared objects) must be found on places
   indicated by the environment variable `LD_LIBRARY_PATH`
   or in the default place, indicated at compilation of the loader.
- The entry point is called like if it is `int main(int argc, char **argcv);`.

## Compatibility

The command line interface by itself can be compiled for any
system that support the Tuim's ELF loader.

## Building

To build the loader you need:

- C compiler (ISO C11 standard compliant).
   - On Unix systems LLVM CLang or GCC are recommended.
   - On Winsdows MSVC is recommended.
- Tuim's ELF loader library.

If you preffer, you can use CMake to build.

```bash
# Buid using CMake
cmake -B .build
cmake --build .build

# Build using LLVM CLang
mkdir .build
cc -fPIE -I../src-loader/include -o .build/tuim tuim.c ../src-loader/.build/libtuim.o
```

## Author

I.F.F. dos Santos --
[e-mail](ismaellxd@gmail.com) --
[GitHub](https://github.com/ismaeldamiao).

## Donation

Did you like the project? Make a donation so that I can continue working on it.

- PayPal: <https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=D66EM3DGU35EE>.

- **Para brasileiros**: Você também pode doar usando PIX, a chave é o e-mail `ismaellxd@gmail.com`.
