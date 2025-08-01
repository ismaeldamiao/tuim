# The Tuim Project

[I. F. F. dos Santos](mailto:ismaellxd@gmail.com)

Last modified: July 23, 2025.

![Crates.io License](https://img.shields.io/crates/l/MIT)
![GitHub code size in bytes](https://img.shields.io/github/languages/code-size/ismaeldamiao/tuim)
![GitHub Release](https://img.shields.io/github/v/release/ismaeldamiao/tuim)

!> Tuim is a project in active development,
for now it is designed only for test purposes.

## Abstract

Tuim, formally The Tuim Project, is a project that aims to improve
both code and binary compatibility.
To archieve this Tuim start by providing a cross platform ELF interpreter,
an Application Binary Interface (ABI)
and some libraries that application may link with it.
This document sumarises how The Tuim Project handle that challenges...

## Introduction

TODO: Complete this section

### Complementar documentation

!> Unlinked documents are planned to be writen on the future.

This document covers only general concepts about The Tuim Project,
each software produced within The Tuim Project is documented
on they own:
- [Tuim's Dynamic Loader and Linker](libtuim/)
- [Tuim's ELF Interpreter](tuim/)
- [Compiler Run time Library](libcrt/)
- [C Standard Library](libc/)

Developers targeting Tuim Environments may refer to
the following complementar documents:
- [The Tuim Interface](tuim_interface.md)
- ABI for object oriented programing languages
- [C Programing Language](c_programing_language.md)
- Rust Programing Language

Developers shall note that if some language
was a standard and open implementation
conforming to this document and the above referenced documents
then that language is _de facto_ compatible with Tuim.
Referece [_missing_] was a curated list of _de facto_ compatible languages.

## Application Binary Interface

TODO: Complete this section

### File format

The file format used by Tuim is the
Executable and Linkable Format (ELF),
online documentation for it can be found at
<https://www.sco.com/developers/gabi/latest/contents.html>.

For the purposes of Tuim's ABI any extension otherwise
the ones expecified on the processor suplement (ps) are ignored,
therefore the `EI_OSABI` field of ELF Identification
usually shall holds `ELFOSABI_NONE`.
Also the `EI_VERSION` field shall holds `EV_CURRENT`.

In order to minimize issues and allow more a flexible and portable environment
this ABI uses only position independent code.
Therefore addresses on object files are interpreted as virtual addresses
and the interpreter shall correct address in order to relocate the code.

Tuim's ELF interpreter is implemented at `tuim` in the source tree
and documentation for it may be found at [tuim/docs](interpreter/).

isso aparece <Jesus>?

### Dynamic loading

The `PT_INTERP` program header element shall be ignored by the interpreter.

For the purposes of this and all documents distributed within The Tuim Project
the default directory used to find for shared objects is denoted as
`${TUIM_HOME}/lib`.

If a executable or shared object
`DT_NEEDED` or `DT_RUNPATH` dynamic tag have one or more slash (`/`)
but does not start with `$ORIGIN` (or `${ORIGIN}`),
then that object is no longer portable.
If that object is placed on a OS it is not build from and the interpreter
load it then the behavior is undefined.

### Application Initialization and termination

### Standard Libraries

## Developing for Tuim

In this section are described concepts and prodecures
used to build libraries and applications for Tuim.

### Development environment

The terminal emulator used almost always runs a POSIX compliant
[shell](https://pubs.opengroup.org/onlinepubs/9799919799/idx/shell.html).
A script placed at `${TUIM_HOME}/environment`
holds environment variables and other useful tools like aliases.
For more details about they usage see `Saying "Hello"` bellow.

Addicional command line utilities used are:

- `man` - <https://pubs.opengroup.org/onlinepubs/9799919799/utilities/man.html>
- `make` - GNU Make (<https://www.gnu.org/software/make/>).

### Assembler and Linker

TODO: Complete this subsection

TODO: Implement the manpage used bellow

Assemble code used is always the written in GNU style syntax.

```bash
man tuim/share/man/ld.1
man tuim/share/man/as.1
```

### Programing languages

TODO: Write this subsection

### MakeFiles

TODO: Write this subsection

### CLI for the ELF interpreter

TODO: Write this subsection

```bash
man tuim/share/man/tuim.1
```

### Saying "Hello"

In order to implement a hello world program a simple
assumption shall be made:
Suppose that there is a shared object `${TUIM_HOME}/libc.so`
holding the definition of the symbol `puts`,
that symbol takes as argument the address of a null-terminated
ASCII string (or UTF-8, if the execution environment support it)
and prints out that string on a standard output device.

Using the C Programing Language the program only needs to call
`puts` taking the string `Hello World!` as argument.
The name used for the entry point symbol does not matter
but `program` is used bellow.
That way, a ASCII encoded file named `hello.c` with the following content
shall be our hello world program.

```c
extern int puts(const char *);

void program(void){
   puts("Hello World!");
}
```

Now the `${TUIM_HOME}/environment` script shall be used to get the
`cc` and `ld` command line utilities, it shall be sourced with:

```bash
. "${TUIM_HOME}/environment"
```

The executable `hello.elf` is obtained from a intermediate file
`hello.o` that is the product of compiling `hello.c`.
That file shall be linked with `${TUIM_HOME}/libc.so` using `-l c` linker flag.

```bash
cc -o hello.o hello.c
ld --entry program -l c -o hello.elf hello.o
```

Finally, the `tuim` command line interface for the ELF interpreter
can be used to execute the application.

```bash
tuim hello.elf
```

## Implementation

### ELF Interpreter

## Conclusion

TODO: Write this section

## References

TODO: Write this section

## Appendex

### Author

The Tuim Project was created by **I. F. F. dos Santos**.

Contact: [ismaellxd@gmail.com](mailto:ismaellxd@gmail.com)

GitHub profle: <https://github.com/ismaeldamiao>

### Donations

Develop The Tuim Project consume a lot of time.
If the project is useful for you or if you want to see new features on it
please consider to donate some money.

- **GitHub Sponsors**: https://github.com/sponsors/ismaeldamiao

- **PayPal**: <https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=D66EM3DGU35EE>.

- **PIX**: `ismaellxd@gmail.com`.

### Join the community

You can join the [Telegram group](https://t.me/tuim_community)
and place on it your questions and comments.
There is also a [Discord server](https://discord.gg/XmHuXpS98A).

If you want to contribute with the project one time then pull a request,
but if want to join the project please send a e-mail
to [ismaellxd@gmail.com](mailto:ismaellxd@gmail.com).

### License

Software produced within the Tuim Project are licenced under the MIT licence.

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
