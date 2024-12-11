# TUIM


> Hi,
>
> Tuim is a project created to improve the portability of compiled machine code,
> yes this is a very good challenge but can be done, with some effort.
> Here you can read about
> the core of the project, if you you can or if you want to use the project for
> commencial purposes consider to place a donation to support my work.
>
> I.F.F. dos Santos

## Table of contents

- [Introduction](#introduction)
- [Tuim's loader](#tuims-loader)
- [Tuim's command line interface](#tuims-command-line-interface)
- [Donation](#donation)
- [License](#license)

## Introduction

Tuim is a open source project and all  the source code is freely available
at [GitHub](https://github.com/ismaeldamiao/tuim/).

## Tuim's loader

Tuim's loader is library written in C,
the only official application programing interface for it
is the C header `<tuim.h>`.
It's designed to be a backend for interfaces that wrap to portable applications.

### C interface

To work with the loader you can the header `<tuim.h>`.

```C
#include <tuim.h>
```

The API defines a opaque type `tuim_elf` to store
a executable or a shared object in memory.

```C
typedef struct tuim_elf_s tuim_elf;
```

The functions declared by the API are:

- `tuim_elf* tuim_loader(const char *file_path)`
   - **Description:**
      Load in the memory a executable or a shared object and its dependencies.
   - **Arguments:**
      The string pointed by `file_path` specify
      the name of the ELF file to be loaded.
   - **Return value:**
      On sucess a pointer to the structure holding the loaded
      ELF file is returned,
      otherwise the null pointer is returned.
- `void* tuim_getentry(const tuim_elf *elf)`
   - **Description:**
      Get the entry point of a executable.
   - **Arguments:**
      The structure pointed by `elf` specify
      the structure holding the executable.
   - **Return value:**
      If the executable have a entry point
      than a pointer to the entry point is returned,
      otherwise the null pointer is returned.
      The null pointer is returned if `elf` is not sucessful
      initialized by `tuim_loader`.
- `void* tuim_findsymbol(const char *symbol, const tuim_elf *elf)`
   - **Description:**
      Find a global symbol, first in the ELF file and then in its dependencies.
   - **Arguments:**
      The string pointed by `symbol` specify the symbol name,
      internaly interpreted as a ASCII string.
      The structure pointed by `elf` specify
      the structure holding the ELF file.
   - **Return value:**
      If the symbol is found than the a pointer to its definition is returned,
      otherwise the null pointer is returned.
      The null pointer is returned if `elf` is not sucessful
      initialized by `tuim_loader`.
- `void tuim_free(tuim_elf *elf)`
   - **Description:**
      Free the memory used to load the ELF file and its dependencies.
   - **Arguments:**
      The structure pointed by `elf` specify
      the structure holding the ELF file.

### Error handling

## Tuim's command line interface

Tuim's command line interface (CLI) is the first front end for the
Tuim's loader.
The CLI is designed to load a executabe and jumps to the entry point.
By default the CLI suppose that at the entry point
there is a function with the C signature of the `main` function.

```C
int main(int argc, char **argv);
```

## Donation

Did you like the project? Make a donation so that I can continue working on it.

- PayPal: <https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=D66EM3DGU35EE>.

- **Para brasileiros**: Você também pode doar usando PIX, a chave é o e-mail `ismaellxd@gmail.com`.

## License

```
MIT License

Copyright (c) 2024 I.F.F. dos Santos <ismaellxd@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```
