# The C Standard Library

The Tuim's C Standard Library -- referenced at runtime by `libc.so` --
is a implementation of the C Standard Library for the freestanding environment
using that Tuim provide.
For more information see the [documentation](doc).

## Dependencies

Build time:
* Tuim's developer kit
* `kernel.h`[^libkernel]

Runtime:
* `libkernel.so`[^libkernel]

[^libkernel]: I.F.F. dos Santos, The Kernel Library, version 0.3, 2025.

## Interfaces

For C Programing Language:
* See headers at [include](include)

## Building

Build for Unix (Linux, Mach-OS, BSD, ...):

```sh
make unix
```

## Donations

Did you like the project? Make a donation so that I can continue working on it.

- **GitHub Sponsors**: https://github.com/sponsors/ismaeldamiao

- **PayPal**: <https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=D66EM3DGU35EE>.

- **PIX**: `ismaellxd@gmail.com`.
