# Olá Mundo!

This program prints "Ola Mundo!" on stadard output.

Dependency:
* `libc.so` -- The C Standard Library.

## Building

> [!NOTE]
> This tutorial assume that the development environment is set as in
> the codumentation of the Tuim project.

Prepare the environment:

```sh
. "${TUIM_HOME}/share/env-host.sh"

mkdir tmp
mkdir bin
```

Build the program:

```sh
cc -o tmp/main.o src/main.c
ld -e main -o bin/ola.elf -l c tmp/main.o
```

Execute it:

```sh
tuim run bin/ola.elf
```

To build for distincts processors use:

```sh
sh scripts/build-all.sh
```

## Donations

Did you like the project? Make a donation so that I can continue working on it.

- **GitHub Sponsors**: https://github.com/sponsors/ismaeldamiao

- **PayPal**: <https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=D66EM3DGU35EE>.

- **PIX**: `ismaellxd@gmail.com`.
