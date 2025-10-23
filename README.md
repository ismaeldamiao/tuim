<h1 align="center">
   <img src="www/assets/img/logo.jpg" alt="The Tuim Project" width="30%">
   <br>The Tuim Project
</h1>

![Crates.io License](https://img.shields.io/crates/l/MIT)
![GitHub code size in bytes](https://img.shields.io/github/languages/code-size/ismaeldamiao/tuim)
![GitHub Release](https://img.shields.io/github/v/release/ismaeldamiao/tuim)

> [!CAUTION]
> Tuim is a project in active development,
> bellow the version 1.0 it is designed only for test purposes.

## About Tuim

The Tuim project aims to provide a cross platform
Application Binary Interface (ABI).
It can be used, for example, to produce applications that can be
executed on Linux, MacOS and Windows without recompilation nor virtualization.

## Introduction

The project was created with the aim of facilitating development for
different systems,
thus decreasing the effort and costs of producing cross-platform applications.
When targeting Tuim developers are allowed to build a object code
that is Operational System (OS) independent.
Various programing languages can be used to target Tuim and distincts
languages can interoperate, therefore developers can use their preferred language.
No similar solutions without virtualization exists.

### How it works?

Supose a executable `ola.elf` that want to print on the terminal the message
"Ola mundo!", it request such service (write some bytes on terminal)
to the execution environment by calling the function `puts`.
The executable `ola.elf` do not know how `puts` is implemented but
it know that it's implemented in the library `libc.so`.
If the execution environment have a `libc.so` library then the function
`puts` can be sucessfully called by the executable and the message
"Ola mundo!" can be written to the terminal.

Since there is no Operational System (OS) dependent code in the executable
`ola.elf` then **any** OS that understand instructions in such executable
can execute it natively, without the need of recompilation or virtualization.
Of course OS dependent instruction can be found in the library `libc.so`,
therefore any OS need to have its own compilation of `libc.so`.

This is how a executable can be "portable" in the sence that more than one
OS can execute it. Some libraries also can be "portable" but a lot of libraries
need to have OS dependent instructions.

In order to turn possible the existence of such kind of "portable" executables
Tuim have a interpreter that read executables and libraries
and load its instructions on the computer's memory
and have also a set of convenctions that executables and libraries need to
follow in order to work together.

## Get started

The following steps shall be used to build the standard distribution:

```sh
. "tuim/scripts/development.sh"

export TUIM_HOME="/opt/tuim"

cd tuim
make TUIM_HOME="${TUIM_HOME}" install
cd ..

. "${TUIM_HOME}/etc/profile.d/development.sh"

cd libkernel
make PREFIX="${TUIM_HOME}" install
cd ..

cd libc
make PREFIX="${TUIM_HOME}" install
cd ..

cd sh
make PREFIX="${TUIM_HOME}" install
cd ..

cd tuim
make PREFIX="${TUIM_HOME}" TUIM_HOME="${TUIM_HOME}" install
cd ..

# now build for the virtual machine
cd tuim
make clean
make riscv-vm
make TUIM_HOME="${TUIM_HOME}/opt/riscv-vm" install
cd ..

. "${TUIM_HOME}/opt/riscv-vm/etc/profile.d/development.sh"
# ... repeat everything else with PREFIX="${TUIM_HOME}/opt/riscv-vm"
```

At the end `${TUIM_HOME}` is expected to look as:

```
[/opt/tuim|C:Program Files\tuim]
|-- bin
|   |-- tuim.elf
|   `-- sh.elf
|-- etc
|   |-- profile
|   `-- profile.d
|       `-- development.sh
|-- include
|   `-- <various files>
|-- lib
|   |-- libc.so
|   `-- libkernel.so
|-- opt
|   `-- riscv-vm
|       |-- <like above>
|       `-- tuim.so
`-- tuim[.exe]
```

You may refer to the [documentation](doc) to get instruction about how
to build and install Tuim.
Documentation is written in LaTeX,
you can build it or buy a compiled PDF at
<https://patreon.com/ismaeldamiao>.

## License

Tuim is released under the MIT License. See the **[LICENSE](LICENSE)** file for details.

## Author

Tuim was created by **[I. F. F. dos Santos](https://github.com/ismaeldamiao)**.

## Join the community

You can join the [Telegram group](https://t.me/tuim_community)
and place on it your questions and comments.

If you want to contribute with the project one time then pull a request,
but if want to join the project please send a e-mail
to [ismaellxd@gmail.com](mailto:ismaellxd@gmail.com).

## Donations

Did you like the project? Make a donation so that I can continue working on it.

- **GitHub Sponsors**: https://github.com/sponsors/ismaeldamiao

- **PayPal**: <https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=D66EM3DGU35EE>.

- **PIX**: `ismaellxd@gmail.com`.

https://riscv.org/blog/2023/01/run-32-bit-applications-on-64-bit-linux-kernel-liu-zhiwei-guo-ren-t-head-division-of-alibaba-cloud/
