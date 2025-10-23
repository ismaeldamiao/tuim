# Tuim Kernel Interface Library

[I. F. F. dos Santos](mailto:ismaellxd@gmail.com)

Last modified: September 09, 2025.

![Crates.io License](https://img.shields.io/crates/l/MIT)
![GitHub code size in bytes](https://img.shields.io/github/languages/code-size/ismaeldamiao/tuim)
![GitHub Release](https://img.shields.io/github/v/release/ismaeldamiao/tuim)

!> Tuim is a project in active development,
for now it is designed only for test purposes.

> The Tuim Kernel Interface Library is a library designed to interface
> with the Operational System Kernel.
>
> Type: Library  
> API language: C  
> Shared Object: `libkernel.so`

## Dependencies

### Build time dependencies

You need a working Tuim's Development Environment,
if Tuim is already installed then you can set up that environment
by typing on a shell section:

```sh
. "${TUIM_HOME}/etc/profile.d/development.sh"
```

### Runtime dependencies

There are no runtime dependency.

## Building from source

You can build the library by typing on the shell section:

```sh
make INCLUDE_DIR=include/${KERNEL}/${ARCH} SOURCE_DIR=src/${KERNEL}/${ARCH}
```

where `${KERNEL}` is any of `linux`, `xnu` or `windows-nt` and
`${ARCH}` is some of `riscv32`, `riscv64`, `arm`, `aarch64`, `i386` or `amd64`,
depending on your OS and architecture.

!> OSes `xnu` and `windows-nt` are not implemented yet.

!> Architectures `riscv32`, `riscv64`, `aarch64`, `i386` and `amd64` are not implemented yet.

## Installing

You can install with:

```sh
make PREFIX="${TUIM_HOME}" install
```

## Appendex

### Author

The Tuim Project was created by **I. F. F. dos Santos**.

Contact: [ismaellxd@gmail.com](mailto:ismaellxd@gmail.com)  
GitHub profle: <https://github.com/ismaeldamiao>

### Donations

Develop The Tuim Project consume a lot of time.
If the project is useful for you or if you want to see new features on it
please consider to donate some money.

* **GitHub Sponsors**: https://github.com/sponsors/ismaeldamiao
* **PayPal**: <https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=D66EM3DGU35EE>.
* **PIX**: `ismaellxd@gmail.com`.

### License

Software produced within The Tuim Project are licenced under the MIT licence.

> Copyright © 2024-2025 I.F.F. dos Santos <ismaellxd@gmail.com>
> 
> Permission is hereby granted, free of charge, to any person obtaining a copy
> of this software and associated documentation files (the "Software"), to deal
> in the Software without restriction, including without limitation the rights
> to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
> copies of the Software, and to permit persons to whom the Software is
> furnished to do so, subject to the following conditions:
> 
> The above copyright notice and this permission notice shall be included in all
> copies or substantial portions of the Software.
> 
> THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
> IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
> FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
> AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
> LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
> OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
> SOFTWARE.
