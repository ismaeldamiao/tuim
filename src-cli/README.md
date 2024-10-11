# Command Line Interface

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

## Build

You can build the loader with CMake.

Requirements:

- CMake (optional)
- a C compiler with support to C23.

```bash
# To buid with CMake
cmake -B .build
cmake --build .build

# To build manually using a POSIX shell
mkdir .build
cc -fPIE -I../src-loader -o .build/tuim tuim.c ../src-loader/.build/libtuim.o
```

## Donation

Did you like the project? Make a donation so that I can continue working on it.

- PayPal: <https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=D66EM3DGU35EE>.

- **Para brasileiros**: Você também pode doar usando PIX, a chave é o e-mail `ismaellxd@gmail.com`.
