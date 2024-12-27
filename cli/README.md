<h1 align="center">Tuim Command Line Interface</h1>

![Crates.io License](https://img.shields.io/crates/l/MIT)

> [!CAUTION]
> This app is in active development. Its interface might change any time without any notice.

## Usage

Given a executable called `app.elf` you can execute it with the command.

```bash
tuim app.elf
```

Some technical requirements, for developers, are:

- The executable must be a ELF file.
- The ELF must be a executable (`ET_EXEC`).
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

## Donation

Did you like the project? Make a donation so that I can continue working on it.

- **GitHub Sponsors**: https://github.com/sponsors/ismaeldamiao

- **PayPal**: <https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=D66EM3DGU35EE>.

- **PIX**: `ismaellxd@gmail.com`.
