# Tuim setup in Termux

This is the step by step tutorial to setup the a complete
Tuim command line environment in [Termux](https://termux.dev/en/).

## Get started

### Prerequisites

Make sure you have `clang` installed.

```sh
# On Ubuntu
sudo apt install -y clang
# On Termux
pkg install clang
```

Note: If you want to use other compiler then set the environment variable
`CC` to the desired compiler.

### Download

For now, there is no download link because tuim is in the beta stage,
but you can download the source tree from GitHub.

The remains parts of this tutorial assume that your are at the project's
root directory in Termux.

## Install

To build and install you may use the `install.sh` script.

```sh
sh scripts/install.sh
```

To force the build step you may use the `build.sh` script just after install.

```sh
sh scripts/build.sh
sh scripts/install.sh
```

By default tuim CLI is installed at `/usr/bin` and its files at `/usr/lib/tuim`,
if you want to use a directory other than `/usr` then
set the environment variable `PREFIX`, for example:

```sh
export PREFIX="${HOME}"
sh scripts/build.sh
sh scripts/install.sh
```

## Donation

Did you like the project? Make a donation so that I can continue working on it.

- PayPal: <https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=D66EM3DGU35EE>.

- **Para brasileiros**: Você também pode doar usando PIX, a chave é o e-mail `ismaellxd@gmail.com`.
