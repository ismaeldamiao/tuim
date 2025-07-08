# The Kernel Library

Tuim's Kernel Library -- referenced at runtime by `libkernel.so` --
is a collection of symbols that provide access to OS specific features.
For more information see the [documentation](doc).

## Dependencies

Build time:
* Tuim's developer kit

Runtime:
* No dependency

## Interfaces

For C Programing Language:
* See the header [kernel.h](./include/kernel.h)

## Get started

Build for Linux:

```sh
make linux
```

Install:

```sh
cp lib/libkernel.so "${TUIM_HOME}/lib/libkernel.so"
cp include/kernel.h "${TUIM_HOME}/include/kernel.h"
```

## Donations

Did you like the project? Make a donation so that I can continue working on it.

- **GitHub Sponsors**: https://github.com/sponsors/ismaeldamiao

- **PayPal**: <https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=D66EM3DGU35EE>.

- **PIX**: `ismaellxd@gmail.com`.
