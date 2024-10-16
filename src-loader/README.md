<h1 align="center">ELF Loader</h1>

> [!CAUTION]
> This project is in beta stage, it may not work as espected
> and it's API may change at any time.

This is the ELF Loader for the Tuim project,
a library with a interface to load ELF files in memory and prepare it
for execution.

## Usage

To use the loader, first you need to include the header.

```c
#include <tuim.h>
```

Then you can can load the executable file and jump to the entry point.

```c
tuim_elf *elf;
void(*entry)(void);

elf = tuim_loader(file_path);
if(elf == NULL){
   // Handle error by check the tuim_error variable.
}
entry = tuim_getentry(elf);
if(entry == NULL){
   // Handle error
}
entry();
```

For more details please look at the code
of interfaces implemented for the `tuim` project.

## Compatibility

- **Operational System:**
   For now the library can be compiled for Unix like systems and
   Windows.
- **Processor:**
   Support to ARM and AMD64 are partially implemented.

## Building

To build the loader you need:

- C compiler (ISO C11 standard compliant).
   - On Unix systems LLVM CLang or GCC are recommended.
   - On Winsdows MSVC is recommended.

If you preffer, you can use CMake to build.

```bash
# Build using CMake
cmake -B .build
cmake --build .build

# Build using LLVM CLang
mkdir .build
clang -c -fPIC -o .build/libtuim.o libtuim.c
```

## Author

I.F.F. dos Santos --
[e-mail](ismaellxd@gmail.com) --
[GitHub](https://github.com/ismaeldamiao).

## Donation

Did you like the project? Make a donation so that I can continue working on it.

- PayPal: <https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=D66EM3DGU35EE>.

- **Para brasileiros**: Você também pode doar usando PIX, a chave é o e-mail `ismaellxd@gmail.com`.
