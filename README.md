<h1 align="center">
   <img src="www/assets/img/logo.jpg" alt="The Tuim Project" width="30%">
   <br>The Tuim Project
</h1>

![Crates.io License](https://img.shields.io/crates/l/MIT)
![GitHub code size in bytes](https://img.shields.io/github/languages/code-size/ismaeldamiao/tuim)
![GitHub Release](https://img.shields.io/github/v/release/ismaeldamiao/tuim)

> [!CAUTION]
> Tuim is a project in active development,
> bellow the version 1.0 it is designed only for test purposes.

## About Tuim

The Tuim project aims to provide a cross platform
Application Binary Interface (ABI).
It can be used, for example, to produce applications that can be
executed on Linux, MacOS and Windows without recompilation nor virtualization.

## Introduction

The project was created with the aim of facilitating development for
different systems,
thus decreasing the effort and costs of producing cross-platform applications.
When targeting Tuim developers are allowed to build a object code
that is Operational System (OS) independent.
Various programing languages can be used to target Tuim and distincts
languages can interoperate, therefore developers can use their preferred language.
No similar solutions without virtualization exists.

### How it works?

Supose a executable `ola.elf` that want to print on the terminal the message
"Ola mundo!", it request such service (write some bytes on terminal)
to the execution environment by calling the function `puts`.
The executable `ola.elf` do not know how `puts` is implemented but
it know that it's implemented in the library `libc.so`.
If the execution environment have a `libc.so` library then the function
`puts` can be sucessfully called by the executable and the message
"Ola mundo!" can be written to the terminal.

Since there is no Operational System (OS) dependent code in the executable
`ola.elf` then **any** OS that understand instructions in such executable
can execute it natively, without the need of recompilation or virtualization.
Of course OS dependent instruction can be found in the library `libc.so`,
therefore any OS need to have its own compilation of `libc.so`.

This is how a executable can be "portable" in the sence that more than one
OS can execute it. Some libraries also can be "portable" but a lot of libraries
need to have OS dependent instructions.

In order to turn possible the existence of such kind of "portable" executables
Tuim have a interpreter that read executables and libraries
and load its instructions on the computer's memory
and have also a set of convenctions that executables and libraries need to
follow in order to work together.

## Get started

You may refer to the [documentation](doc) to get instruction about how
to build and install Tuim.
Documentation is written in LaTeX,
you can build it or buy a compiled PDF at
<https://patreon.com/ismaeldamiao>.

### Project structure

The project structure is:

```
<The Tuim Project root directory>
|-- CITATION.cff
|-- LICENSE
|-- README.md
|-- libc                C standard library
|-- libcrt              compiler runtime library
|-- libkernel           routines calling the kernel
|-- tuim                ELF interpreter and their CLI
`-- www                 website
```

Except for `www`, sub directories on root directory follow the general
structure:

```
<The Tuim Project root directory>
`-- <sub directory>
    |-- Makefile
    |-- README.md
    |-- bin             scripts and precompiled portable executables, if any
    |-- docs            documentation
    |-- include         application programing interfaces
    |-- lib             precompiled portable shared objects, if any
    |-- share           ...
    |   `-- man         manual pages
    `-- src             source code
```

Documentation are written in MarkDown and may be displayed on a web browser
using `docsify`, serve it with:

```bash
docsify serve <sub directory>/docs
```

## License

Tuim is released under the MIT License. See the **[LICENSE](LICENSE)** file for details.

## Author

Tuim was created by **[I. F. F. dos Santos](https://github.com/ismaeldamiao)**.

## Join the community

You can join the [Telegram group](https://t.me/tuim_community)
and place on it your questions and comments.

If you want to contribute with the project one time then pull a request,
but if want to join the project please send a e-mail
to [ismaellxd@gmail.com](mailto:ismaellxd@gmail.com).

## Donations

Did you like the project? Make a donation so that I can continue working on it.

- **GitHub Sponsors**: https://github.com/sponsors/ismaeldamiao

- **PayPal**: <https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=D66EM3DGU35EE>.

- **PIX**: `ismaellxd@gmail.com`.
