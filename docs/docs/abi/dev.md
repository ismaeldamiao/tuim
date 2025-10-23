# Tuim's Development environment

[I. F. F. dos Santos](mailto:ismaellxd@gmail.com)

Last modified: September 11, 2025.

![Crates.io License](https://img.shields.io/crates/l/MIT)
![GitHub code size in bytes](https://img.shields.io/github/languages/code-size/ismaeldamiao/tuim)
![GitHub Release](https://img.shields.io/github/v/release/ismaeldamiao/tuim)

!> Tuim is a project in active development,
for now it is designed only for test purposes.

## Abstract

This article exposes the convenctions followed
in the development of The Tuim Project
and how developers may follow that conventions to target environments
based on Tuim.

## Introdution

Tuim, formally The Tuim Project,
is a project focused on code portability
and to archieve this a cross-platform development environment shall be
standardized.[1]

Explicitly, developers that want to build Tuim related source code
and code following convenctions on this article
shall be provided with a POSIX compliant shell,
also the following shell utilities from POSIX shall be availablle:[2]

```
cp    rm    sh    mkdir    ls
```

Also, the following additional shell utilities,
wrose behavior is described bellow, shall be available
-- the first column utilities shall be implemented as aliases and
seccond column utilities shall be implemented as functions:

```
ld    make     lua
as
```

Also, the following environment variables,
wrose behavior is described bellow, shall be available:

```
LD    LDFLAGS
ASM   ASMFLAGS
```

Complementar documentation shall standardize others
shell utilities and environment variables
in order to extend the development environment for languages.

Also, if the environment variable `TUIM_HOME` is available then the files
`${TUIM_HOME}/etc/profile.d/development.sh` and
`${TUIM_HOME}/development.sh` shall be available.
The first file may be sourced to set up
environment variables, aliases and functions
suitable to build projects when targeting the Tuim ABI,
the seccond may be sourced to build projects when targeting the native OS ABI.

These tools are required to standardize the build process using
Make Scripts decribed bellow.
For example, library and application code from Tuim are compiled and installed
using:

```sh
. "${TUIM_HOME}/etc/profile.d/development.sh" # if not done yet
make PREFIX="${TUIM_HOME}"
make PREFIX="${TUIM_HOME}" install
```

## Utilities and environment variables

This section sumarises the behavior and usage of
utilities and environment variables
referenced at the Introduction.

### Lua interpreter

The shell utility `lua` shall serve a Lua interpreter. [3]

### Linker

Environment variables `LD` and `LDFLAGS`
may be used to define a link editor as:

```sh
alias ld="${LD} ${LDFLAGS}"
```

The link editor is described in the source tree of The Tuim Project
in the `skel/share/man/ld.1` manual page.

### Assembler

Environment variables `ASM` and `ASMFLAGS`
may be used to define a assembler as:

```sh
alias as="${ASM} ${ASMFLAGS}"
```

The assembler is described in the source tree of The Tuim Project
in the `skel/share/man/as.1` manual page.

## Make Scripts

The `make` utility is a function to wrap around commands used to run
build automation scripts.
On this article that scripts are called Make Scripts,
Make Scripts are designed to provide a simple command line interface
to build, install and pack software and shall be implemented
using utilities and tools from this article.

### Language

The recommended language used by Make Scripts is the  Shell Command Language,
Make Scripts shall use the shebang `#!/bin/sh` and shall be named `Make.sh`.

Also, the Lua Programing Language may be used, in that case
Make Scripts shall use the shebang `#!/bin/lua` and shall be named `Make.lua`.

Usually Make Scripts are called acconding to name extension,
like `sh Make.sh`. The `make` is a function to avoid writting
the language interpreter and name extension:

```sh
make ( ) {
   if   [ -r "Make.sh"  ]; then
      sh  Make.sh  $@; return $?
   elif [ -r "Make.lua" ]; then
      lua Make.lua $@; return $?
   fi
   echo "ERROR: No Make Script found."
   return 1
}
```

### Commands

Using the above function wrapper, the Make Script usage look as:

```
make [macro=value...] [command]
```

Supported commands shall be at least:
* `install`: Execute the install step.
* `clean`: Remove/delete the build directory.

If no command are specified then the build step shall be executed.

Supported macros shall be at least:
* `SOURCE_DIR`: Where the script finds for source files. Defaults to `src`.
* `INCLUDE_DIR`: Where the script finds for interface files. Defaults to `include`.
* `BUILD_DIR`: Where the script places generated files. Defaults to `build`.
* `BUILD_TYPE`: The build type, shall be `debug|release`. Defaults to `release`.
* `STATIC_LIBRARY_PREFIX`: The prefix used for static libraries. Defaults to `lib`.
* `SHARED_LIBRARY_PREFIX`: The prefix used for shared libraries. Defaults to `lib`.
* `STATIC_LIBRARY_SUFFIX`: The suffix used for static libraries. Defaults to `.a`.
* `SHARED_LIBRARY_SUFFIX`: The suffix used for shared libraries. Defaults to `.so`.
* `EXECUTABLE_SUFFIX`: The suffix used for executables. Defaults to `.elf`.
* `DESTDIR`: Used to replace the root. Defaults to the null character.
* `PREFIX`: The prefix used to install. Defaults to `${DESTDIR}/usr/local`.
* `BINDIR`: Where the script places executables. Defaults to `${PREFIX}/bin`.
* `DATADIR`: Where the script places data and configurations. Defaults to `${PREFIX}/share`.
* `MANDIR`: Where the script places manual pages. Defaults to `${DATADIR}/man`.

If a macro is set more than once
then the last definition shall be used,
this allow the `make` function to be implemented with different defaults.

!> FIXME: There are no way for build scripts to prevent rebuild from
unchanged sources.

!> FIXME: There are no packing command and no tools to provide it.

## References

[1] I.F.F dos Santos, _The Tuim Project_, [version 0.4](https://github.com/ismaeldamiao/tuim).  
[2] IEEE/Open Group Standard for Information Technology, _Portable Operating System Interface (POSIX™) Base Specifications, Issue 8_, [IEEE Std 1003.1-2024](https://ieeexplore.ieee.org/document/10555529).  
[3] Lua.org, PUC-Rio, _Lua 5.4 Reference Manual_, [Lua 5.4](https://www.lua.org/manual/5.4/)

## Appendex

### Author

The Tuim Project was created by **I. F. F. dos Santos**.

Contact: [ismaellxd@gmail.com](mailto:ismaellxd@gmail.com)  
GitHub profle: <https://github.com/ismaeldamiao>

### Donations

Develop The Tuim Project consume a lot of time.
If the project is useful for you or if you want to see new features on it
please consider to donate some money.

* **GitHub Sponsors**: https://github.com/sponsors/ismaeldamiao
* **PayPal**: <https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=D66EM3DGU35EE>.
* **PIX**: `ismaellxd@gmail.com`.

### Join the community

You can join the [Telegram group](https://t.me/tuim_community)
and place on it your questions and comments.
There is also a [Discord server](https://discord.gg/XmHuXpS98A).

If you want to contribute with the project one time then pull a request,
but if want to join the project please say it at the
[Discord server](https://discord.gg/XmHuXpS98A).

### License

Software produced within The Tuim Project are licenced under the MIT licence.

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
