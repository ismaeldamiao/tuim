# Tuim setup in Termux

This is the step by step tutorial to setup the a complete
Tuim command line environment in [Termux](https://termux.dev/en/).

## Get started

### Prerequisites

First you need to install the prerequisites.

```bash
pkg install clang
```

### Download

For now, there is no download link because tuim is in the beta stage,
but you can download the source tree from GitHub.

The remains parts of this tutorial assume that your are at the project's
root directory in Termux.

### Prepare the environment

A good place put Tuim files is in `usr/lib/tuim`,
you can make a environment variable for this
and make the directory.

```bash
TUIM_HOME=$PREFIX/lib/tuim
mkdir -p $TUIM_HOME/{bin,lib}
```

## Build

### Building the loader

Build the [loader](../src-loader) as usual,
but also specify the place where you want to install Tuim.

```bash
cd src-loader
mkdir .build
clang -c -fPIC -DTUIM_HOME="\"$TUIM_HOME\"" -o .build/libtuim.o libtuim.c
cd ..
```

### Building the CLI

Now you need to build the commad line interface.

```bash
cd src-cli
mkdir .build
clang -fPIE -I../src-loader -o .build/tuim tuim.c ../src-loader/.build/libtuim.o
cd ..
```

## Install

All you need now is copy the files for the desidered places.

```bash
cp src-cli/.build/tuim $PREFIX/bin
```

## Donation

Did you like the project? Make a donation so that I can continue working on it.

- PayPal: <https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=D66EM3DGU35EE>.

- **Para brasileiros**: Você também pode doar usando PIX, a chave é o e-mail `ismaellxd@gmail.com`.
