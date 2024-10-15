
<h1 align="center">Tuim</h1>

> [!CAUTION]
> This project is in beta stage, it may not work as espected
> and it's API may change at any time.

With Tuim you can execute your programs.

Have two or more operational systems installed on your machine?
The Tuim's loader can execute the same binary on all these OS.

## The project

- **ELF Loader**: Tuim have a [library](src-loader)
   with a interface to load ELF files in memory and prepare it
   for execution.
- **CLI**: The main interface for the ELF loader is the
   [command line interface](src-cli), to execute programs from the
   command line.

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
