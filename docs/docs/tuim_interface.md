# The Tuim Interface

[I. F. F. dos Santos](mailto:ismaellxd@gmail.com)

Last modified: July 22, 2025.

## Abstract

The Tuim Interface is designed by Tuim's developer
to handle architecture dependent and system dependent code implementation.
It's a C Programing Language interface plus a shared library.
This document exposes the expected behavior of that interfaces
and they usage within The Tuim Project.

## API

### Headers

```c
#include <tuim/arch.h>   // compilation target machine's architecture
#include <tuim/kernel.h> // compilation target machine's OS kernel
```

### Definitions

The header `<tuim/arch.h>` shall define:

```c
#define tuim_unknown          0x0000
#define tuim_riscv32          0x0001
#define tuim_riscv64          0x0002
#define tuim_arm              0x0003
#define tuim_aarch64          0x0004
#define tuim_i386             0x0005
#define tuim_amd64            0x0006

#define tuim_arch             /* some of above */
```

The header `<tuim/kernel.h>` shall define:

```c
#define tuim_unknown          0x0000
// from 0x0001 to 0x000f, inclusive, are reserved for future use
#define tuim_linux            0x0011
#define tuim_xnu              0x0021
#define tuim_windows_nt       0x0031

#define tuim_kernel           /* some of above */
```

### Functions

!> All functionality specified in this subsection are only partially implemented
by The Tuim Project.

The header `<tuim/arch.h>` shall declare no function protopyte.

The header `<tuim/kernel.h>` shall contain protopytes for
routines executed by the kernel, namelly:

- `tuim_kernel == tuim_linux`:
   Prototypes from System Calls Manual. [1]
- `tuim_kernel == tuim_xnu`:
   Prototypes for System Calls. [2]
- `tuim_kernel == tumi_windows_nt`:
   Thats a bit more complicated since Microsoft does not document
   Windows internals.
   For now the plan is that `<tuim/kernel.h>` shall contains prototypes
   for Nt version of Zw routines from Windows Driver Kit. [3]

## ABI

### Shared libraries

Code using `<tuim/arch.h>` do not need aditional dependency at run time.

Code using `<tuim/kernel.h>` shall be linked with
`libkernel.so` if any symbol from `<tuim/kernel.h>` is referenced.

## Usage examples

The headers `<tuim/arch.h>` and `<tuim/kernel.h>`
are used as a simple and *implementation agnostic*
alternative to choose code for distinct architecture and system
at preprocessor time. For example:

```c
#if tuim_arch == tuim_riscv23
   /* code for riscv 32 bits */
#endif

// or

#if tuim_kernel == tuim_linux
   /* code for systems using the Linux kernel */
#endif
```

Its also useful at compile time (if the compiler can optimize):

```c
if(tuim_arch == tuim_aarch64){
   /* code for Arm Architecture 64 bits */
}

// or

if(tuim_kernel == tuim_xnu){
   /* code for systems using the Xnu kernel */
}
```

The author recomends to never install these heades on
system search directories, such that the presence of that headers
may be used to assert that the compilation is targeting a Tuim environment,
also is recomended:

```c
#if __has_include(<tuim/kernel.h>)
   /* targeting tuim environments */
#else
   /* something else */
#endif
```

If the use of that headers appears useful even when
the compilation is not targeting Tuim environments then a better approach
is to get a copy of that headers and distribute within the code.
General implementation may be found on `tuim/include` directory
at The Tuim Project source tree.
For example the Tuim's ELF interpreter source code use that generic headers.

Since the shared library `libkernel.so` shall be placed
on standard library search directory for Tuim environments,
then linking with it is a quite simple, add `-l kernel` linker flags,
for example `libc.so` is linked as:

```sh
ld --shared -l kernel -o lib/libc.so ${OBJECTS}
```

## Conclusion

TODO: Write this section

## References

1. _Section 2 man pages_, <https://www.commandlinux.com/man-page/man2/>.
2. _syscalls.master_, <https://github.com/apple-oss-distributions/xnu/blob/main/bsd/kern/syscalls.master>.
3. _Windows Kernel - Windows Driver Kit_, <https://learn.microsoft.com/en-us/windows-hardware/drivers/ddi/_kernel/>.
