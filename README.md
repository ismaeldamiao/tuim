<h1 align="center">Tuim</h1>

<div align="center">With Tuim you can execute your programs.</div>

![Crates.io License](https://img.shields.io/crates/l/MIT)
![GitHub code size in bytes](https://img.shields.io/github/languages/code-size/ismaeldamiao/tuim)
![GitHub Release](https://img.shields.io/github/v/release/ismaeldamiao/tuim)

> [!CAUTION]
> This project is in beta stage, it may not work as espected
> and it's API may change at any time.

Have two or more operational systems installed on your machine?
The Tuim's loader can execute the same binary on all these OS.

## The project

- **ELF Loader**:
   The Tuim's [loader](src-loader) is a library that provides
   a application programing interface (API)
   to load ELF files in memory and prepare it for execution.
- **CLI**:
   The Tuim's [command line interface](src-cli)
   is a tool to execute programs from the command line.

## Compatibility

The source code was written to be compiled with LLVM CLang
or with the system's host compiler.

The supported operational systems are unix like systems,
such as Debian and Android,
ReactOS and Windows.

The supported architectures are ARM, x86 and Risc V families.

## Author

I.F.F. dos Santos --
[e-mail](ismaellxd@gmail.com) --
[GitHub](https://github.com/ismaeldamiao).

## Donation

Did you like the project? Make a donation so that I can continue working on it.

- PayPal: <https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=D66EM3DGU35EE>.

- **Para brasileiros**: Você também pode doar usando PIX, a chave é o e-mail `ismaellxd@gmail.com`.
