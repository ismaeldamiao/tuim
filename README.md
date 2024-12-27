<h1 align="center">Tuim</h1>

<div align="center">Interoperability and portability solutions.</div>

![Crates.io License](https://img.shields.io/crates/l/MIT)
![GitHub code size in bytes](https://img.shields.io/github/languages/code-size/ismaeldamiao/tuim)
![GitHub Release](https://img.shields.io/github/v/release/ismaeldamiao/tuim)

> [!CAUTION]
> This library is in active development. Its API might change any time without any notice.

## About the project

This project aims to provide some simple solutions to improve
software portability and language interoperability.
For this I designed tools to build and execute software, as:

- **LibTuim**:
   [LibTuim](libtuim) is a library to load ELF files in memory
   and prepare it for execution.
   LibTuim provide a C API in order to allow the design of loaders with it.
- **CLI**:
   The Tuim's [command line interface](cli)
   is a tool to execute programs from the command line.
- **TDK**:
   The [Tuim Developer Kit](tdk) provide tools to build
   executables and shared objects suitable to run in environments
   with loaders based on LibTuim.

The others softwares in the root of this repository
are libraries refered in the documentation of above tools:

- **CRT Library**:
   The [Compiler RunTime Library](crt) is a system library that compilers
   may use in order to implement language features.
   Compiled code usually must be linked with it.
- **LibC**:
   The [C Standard Library](libc) is the system library of the
   C programing language.
   Code compiled from C usually must be linked with it.

## Design

The key idea is provide binary code that can be loaded and executed
in diferent systems without recompilation or virtual machines,
ensuring portability without loss of performance.

Tuim Environments differenciate
system libraries from portable libraries.
System libraries may use the system call interface
and the system API but must provide a abstraction layer for the user
in the form of a API to abstract the system interface.
On the other hand portable libraries and executables
can interface with and only with others portable libraries and
systems libraries, such binaries never use directly
the system API or software interrupts.
Following these convenctions portable executables runs
in distincts systems that can execute the same intructions,
provided that needed libraries for dynamic linking are present.

## About the author

My name is Ismael,
I'm a brasilian physicist.
You can read more about me at
[GitHub Sponsors](https://github.com/sponsors/ismaeldamiao).

## Join the community

You can join the [Telegram group](https://t.me/tuim_community)
and place on it your questions and comments.

If you want to contribute with the project one time then pull a request,
but if want to join the project please send a e-mail
to [ismaellxd@gmail.com](mailto:ismaellxd@gmail.com).

## Donation

Did you like the project? Make a donation so that I can continue working on it.

- **GitHub Sponsors**: https://github.com/sponsors/ismaeldamiao

- **PayPal**: <https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=D66EM3DGU35EE>.

- **PIX**: `ismaellxd@gmail.com`.
