# Tuim's Dynamic Loader and Linker

[I. F. F. dos Santos](mailto:ismaellxd@gmail.com)

Last modified: July 23, 2025.

> This is the dynamic loader and dynamic linker used by The Tuim Project.

## Get started

TODO: Write this section.

### Building

To build the library for Tuim Environments
you need `libc.so` and the Developer Interface for Tuim.
Build it with the command:

```sh
make libtuim.so
```

If otherwise you are building for the host execution environment
then you shall refer to the proper documentation:
- [Bulding for Linux](linux.md)
- [Bulding for Mac](mac.md)
- [Bulding for Windows](windows.md)

### Installing

For Tuim Environments,
if the `make` command was executed without errors
then it generates the file
`lib/libtuim.so`, install it with:

```sh
cp lib/libtuim.so "${TUIM_HOME}/lib/libtuim.so"
```

The interface for the C Programing Language may be installed with:

```sh
cp include/tuim.h "${TUIM_HOME}/include/tuim.h"
```

## API

### Macros

```c
#define TUIM_VERSION_MAJOR /* see bellow */
#define TUIM_VERSION_MINOR /* see bellow */
```

### typedefs

```c
typedef struct tuim_ctx_s tuim_ctx;
```

The type `tuim_ctx` is a object that stores a information
called _execution context_.
Libraries and application are loaded and linkend in a given
context of execution and code on the same execution context
share some resources like the address space.
If code on distincts execution contexts share these resources
is unspecified.

Any execution context store a status,
a inernal integer `errno` for error code and
a null terminated string `strerror` for a error description.

### The `tuim_newcontext` function

**Synopsis**

```c
#include <tuim.h>
tuim_ctx *tuim_newcontext(
   const char *tuim_home,
   const char *ld_library_path,
   const char *machine
);
```

**Description**

The `tuim_newcontext` function is used to create a new
execution context.

The `tuim_home` variable shall point to a null terminated string
holding a directory suitable to be used a prefix
to find executables and shared objects.
Usually it points to the first character of
the environment variable `TUIM_HOME`.

The `ld_library_path` variable shall point to a null terminated string
holding a list of directories to find shared objects.
Usually it points to the first character of
the environment variable `LD_LIBRARY_PATH`.

The null terminated string pointed to by `machine`
sets the logic to find executables and shared objects.
Instead of use `<tuim_home>` is used `<tuim_home>/<machine>`
as prefix. On that prefix directory a shared object
`tuim.so` shall serve the Dynamic Load and Linker
with routines, as speficied in [nowhere].
If `machine` points to `NULL` then the host machine shall be used to execute
any code loaded on the execution context.

**Returns**

On sucess the function `tuim_newcontext` return a pointer to a
execution context.
If memory allocation for the execution context fails `NULL` is returned,
if some other error occur then it is handled on the execution context.

### tuim_deletecontext

```c
void tuim_deletecontext(tuim_ctx *ctx);
```

The `tuim_deletecontext` shall release all resources used by the
execution context pointed to by `ctx`.

### tuim_getpathexec

```c
char *tuim_getpathexec(tuim_ctx *ctx, const char *elf_name);
```

The `tuim_getpathexec` function return a null terminated string
holding the path of a executable suitable computed using the
null terminated string pointed to by `elf_name`.
If `elf_name` have one or more slashes (`/`) no computation are made,
otherwise the returned value shall be in the form
`<home>/bin/<elf_name>.elf` where `home` is the string used
to create the execution context.

### tuim_getpathdyn

```c
char *tuim_getpathdyn(tuim_ctx *ctx, const char *elf_name);
```

The `tuim_getpathdyn` function return a null terminated string
holding the path of a executable suitable computed using the
null terminated string pointed to by `elf_name`.
If `elf_name` have one or more slashes (`/`) no computation are made,
otherwise the returned value shall be in the form
`<home|ld_library_path>/lib/<elf_name>` where `home` is the string used
to create the execution context and `ld_library_path` are one
path on the respective string used to create the execution context.
Successive call shall run over all possibilities and `NULL`,
always in the same order.

The following snippet ilustrate they usage.

```c
find_shared_object: elf_path = tuim_getpathdyn(ctx, elf_name);
if(elf_path == NULL) /* can't find it */ ;
tuim_loader(ctx, elf_path);
if(tuim_errno(ctx) == TUIM_ENOENT) goto find_shared_object;
```

### The `tuim_loader` function

**Synopsis**

```c
#include <tuim.h>
void tuim_loader(tuim_ctx *ctx, const char *elf_path);
```

**Description**

The `tuim_loader` function
load the ELF file wrose the full path is
the null terminated string pointed to by `elf_path`.
If that file was already loaded then
`tuim_loader` function unload it and then load it again,
this is useful when the file was updated.

The object pointed to by `ctx`
shall be previously initialized by `tuim_newcontext`
and not yet released by `tuim_deletecontext`,
otherwise the result is undefined.

### The `tuim_linker` function

**Synopsis**

```c
#include <tuim.h>
void tuim_linker(tuim_ctx *ctx, const char *elf_path);
```

**Description**

The `tuim_linker` function
relocate the ELF file wrose the full path is
the null terminated string pointed to by `elf_path`.
After the relocation all symbol references
shall refer to the correct place where the symbol is defined.

The object pointed to by `ctx`
shall be previously initialized by `tuim_newcontext`
and not yet released by `tuim_deletecontext`,
otherwise the result is undefined.

If the ELF file was not already loaded then the `tuim_linker` function
call the `tuim_loader` function to load it,
also the `tuim_loader` function is called to load all
shared objects dependencies of the ELF file.

### The `tuim_exec` function

**Synopsis**

```c
#include <tuim.h>
int tuim_exec(
   tuim_ctx *ctx,         /* execution context */
   const char *exec_path, /* path of ELF executable */
   char *const argv[],    /* argument vector */
   char *const envp[]     /* environment pointer */
);
```

**Description**

The `tuim_exec` function execute the ELF executable wrose the full path is
the null terminated string pointed to by `elf_path`.

The machine initial state is prepared by the `tuim_exec` function
like specified in System V ABI processor supement,
except that no axiliary vector entry in provided.
That is, the specification is followed to serve the application
whith arguments and environment variables.

The following table ilustrates how, in general, looks the
stack state at statup.

|               Address              |        Value        |
|:----------------------------------:|:-------------------:|
|                                    | environment strings |
|                                    |   argument strings  |
|                 ...                |         ...         |
| stack + (argc + 2) * sizeof(void*) |       envp[0]       |
| stack + (argc + 1) * sizeof(void*) |      argv[argc]     |
|                 ...                |         ...         |
|        stack + sizeof(void*)       |        arg[0]       |
|                stack               |         argc        |

Where, for purposes of that table,
`sizeof(void*)` is the pointer size of the largest pointer size
supported by the processor suplement.

The object pointed to by `ctx`
shall be `NULL` or previously initialized by `tuim_newcontext`
and not yet released by `tuim_deletecontext`,
otherwise the result is undefined.
If `NULL` then the environment variables `TUIM_HOME` and `LD_LIBRARY_PATH`
are searched on `envp` and are used to call `tuim_newcontext`.

If the ELF file was not already loaded then the `tuim_exec` function
call the `tuim_loader` function and the `tuim_linker` function
to load and link the file.

**Returns**

If the application return then the `tuim_exec` function
return their exit status.
Note that if the application do not provide a exit status
then the teruned value is unspecified.

### tuim_errno

```c
int tuim_errno(tuim_ctx *ctx);
```

The function `tuim_errno` return the `errno` value of
the execution context pointed to by `ctx`.

### tuim_strerror

```c
char *tuim_strerror(tuim_ctx *ctx);
```

The function `tuim_strerror` return `strerror` value of the
execution context pointed to by `ctx`.


**Synopsis**
**Description**
**Returns**

## Donations

TODO: Write this section.

## Licence

> Copyright © 2024-2025 I.F.F. dos Santos <ismaellxd@gmail.com>
> 
> Permission is hereby granted, free of charge, to any person obtaining a copy
> of this software and associated documentation files (the "Software"), to deal
> in the Software without restriction, including without limitation the rights
> to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
> copies of the Software, and to permit persons to whom the Software is
> furnished to do so, subject to the following conditions:
> 
> The above copyright notice and this permission notice shall be included in all
> copies or substantial portions of the Software.
> 
> THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
> IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
> FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
> AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
> LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
> OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
> SOFTWARE.
