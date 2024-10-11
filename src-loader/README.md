# ELF Loader

This is the ELF Loader for the Tuim project,
a library with a interface to load ELF files in memory and prepare it
for execution.

## Build

You can build the loader with CMake.

Requirements:

- CMake (optional)
- a C compiler with support to C23.
- `archive`, if static.

```bash
# To buid with CMake
cmake -B .build
cmake --build .build

# To build manually using a POSIX shell
mkdir .build
cc -c -fPIC -o .build/libtuim.o libtuim.c
ar src .build/libtuim.a .build/libtuim.o # If static
cc --shared -o .build/libtuim.so .build/libtuim.o # If dynamic
```

## Status

For now the ELF Loader is in the beta stage of development
and does not provide a stable software,
but that will change on the first alpha release.

## Donation

Did you like the project? Make a donation so that I can continue working on it.

- PayPal: <https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=D66EM3DGU35EE>.

- **Para brasileiros**: Você também pode doar usando PIX, a chave é o e-mail `ismaellxd@gmail.com`.
