# Tuim's ELF Interpreter

[I. F. F. dos Santos](mailto:ismaellxd@gmail.com)

Last modified: July 23, 2025.

> This is the official ELF interpreter used by The Tuim Project.

## Get started

### Building

Place the file `libtuim.o` on the directory `lib`
and the header `tuim.h` on the directory `include`,
then compile the file `src/main.c`. For example:

```sh
cp -r ../libtuim/lib .
cp -r ../libtuim/include .
mkdir -p bin

# Unix-like
clang -I include -o bin/tuim src/main.c lib/libtuim.o

# Windows
clang -I include -L lib -l tuim -o bin/tuim.exe src/main.c
```

### Installing

NOTE:
This section use some environment variables that may change beteween
installations, you can see recomended values on appendex at the end
of this document.

Create a directory to install Tuim and copy the executable to it,
you also need to add provide a link to it
or add it to the `PATH` environment variable.
For example:

```sh
mkdir -p "${TUIM_HOME}"
cp bin/tuim "${TUIM_HOME}/tuim"
ln -s "${TUIM_HOME}/tuim" "${PREFIX}/bin/tuim"
```

Or, on Windows:

```sh
# ???
```

## Usage

TODO: Write this section.

## Donations

Did you like the project? Make a donation so that I can continue working on it.

- **GitHub Sponsors**: https://github.com/sponsors/ismaeldamiao

- **PayPal**: <https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=D66EM3DGU35EE>.

- **PIX**: `ismaellxd@gmail.com`.

## Appendex

### Recommended installation on Linux

`TUIM_HOME`:
- `/opt/tuim`: For all users installation
- `${HOME}/tuim`: For one user installation
- `${PREFIX}/opt/tuim`: For installation on Termux

`PREFIX`:
- `/usr/local/bin`: For all users installation
- `${HOME}/bin`: For one user installation

### Recommended installation on Mac

TODO: Write this appendex.

### Recommended installation on Windows

TODO: Write this appendex.
