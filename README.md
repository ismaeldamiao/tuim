# Tuim

With Tuim you can execute your programs.

Have two or more operational systems installed on your machine?
The Tuim's loader can execute the same binary on all these OS.

## The project

- **Loader**: Tuim have a [library](src-loader)
   with a interface to load ELF files in memory and prepare it
   for execution.
- **CLI**: The main interface for the loader is the
   [command line interface](src-cli), to execute programs from the
   command line.

## Compatibility

The source code was written to be compiled with LLVM CLang
or with the system's host compiler.

The supported operational systems are unix like systems,
such as Debian and Android,
ReactOS and Windows.

The supported architectures are ARM, x86 and Risc V families.

## Status

For now Tuim is in the beta stage of development and does not provide a
stable software, but that will change on the first alpha release.

## Donation

Did you like the project? Make a donation so that I can continue working on it.

- PayPal: <https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=D66EM3DGU35EE>.

- **Para brasileiros**: Você também pode doar usando PIX, a chave é o e-mail `ismaellxd@gmail.com`.
