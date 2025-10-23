# Tuim's ELF Interpreter

> [!CAUTION]
> The Tuim Project is a work in progress.
> For now it is designed only for test purposes.

This is the reference implementation of the
cross-platform ELF interpreterter. [1]

## Building and installing

The Tuim's ELF Interpreter uses a *Make Script* to help the build for all
platforms.
Note that the `make` command used here behaves distinct from that used on
Unix environments. [2]

The general command to build the ELF Interpreter is:

```sh
make INCLUDE_DIR=include/${KERNEL}/${ARCH} SOURCE_DIR=src/${KERNEL}/${ARCH}
```

where `${KERNEL}` and `${ARCH}` shall be replaced by your OS kernel and
architecture.

For now available kernels are:
- `linux`

And available architectures are:
- `arm`

After a successful build you can install the ELF Interpreter with:

```sh
make install
```

A successful installation may be checked with:

```sh
tuim -h
```

## References

1. I. F. F. dos Santos, *Tuim: A Portable Application Binary Interface*, The Tuim Project, 2025.
2. I. F. F. dos Santos, *A Development Environment for Source Code Distribution*, The Tuim Project, 2025.

## Donations

Did you like the project? Make a donation so that I can continue working on it.

- **GitHub Sponsors**: https://github.com/sponsors/ismaeldamiao
- **PayPal**: <https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=D66EM3DGU35EE>.
- **PIX**: `ismaellxd@gmail.com`.

