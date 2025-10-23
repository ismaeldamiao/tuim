# Tuim

## Building

### Building for Linux

```sh
make INCLUDE_DIR=include/linux/arm SOURCE_DIR=src/linux/arm && make install
tuim bin/arm/ola.elf
```

### Building for Mac

### Building for Windows

### Building for Android

## Installing

```sh
make TUIM_HOME="/opt/tuim" install # for Linux
```

## Trash

```sh
curl -fsSL https://ismaeldamiao.github.io/tuim/install.sh | sh

# exemples of command line requests:
tuim ./ola.elf                 # find for ${PWD}/ola.elf
tuim sh -c "echo Olá Mundo!"   # find for ${TUIM_HOME}/bin/sh.elf
tuim -b riscv-vm game          # find for ${TUIM_HOME}/opt/riscv-vm/{tuim.so,bin/game.elf}
```

https://semver.org/
