<h1 align="center">The Tuim Project</h1>

![Crates.io License](https://img.shields.io/crates/l/MIT)
![GitHub code size in bytes](https://img.shields.io/github/languages/code-size/ismaeldamiao/tuim)
![GitHub Release](https://img.shields.io/github/v/release/ismaeldamiao/tuim)

> [!CAUTION]
> This library is in active development. Its API might change any time without any notice.

## How it works

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

In order to build and install a default setup for use
the script [scripts/build.sh](scripts/build.sh),
the first argument is the parent directory where Tuim must be installed,
the second is your architecture,
the third is the OS.

For example:

```sh
sh scripts/build.sh "${HOME}" x86_64 linux
CC=clang sh scripts/build.sh "${PREFIX}/opt" armv7 linux
```

## Repository structure

*  **doc**:

   The [doc](doc) directory contain code suitable to build the documentation.

*  **interfaces**:

   The [interfaces](interfaces) directory contain code suitable to build
   user interfaces for the ELF interpreter,
   allowing the execution of programs.

*  **libraries**:

   The [libraries](libraries) directory contain code suitable to build some,
   but not all, libraries that can be requested by executables.

*  **example**:

   The [examples](examples) directory contain code suitable to do
   some basic tests and to provide examples of how develop new programs.

*  **skel**:

   The [skel](skel) directory contain some utility files,
   suitable for developers.

## About the author

My name is Ismael,
I'm a brasilian physicist.
You can read more about me at
[GitHub Sponsors](https://github.com/sponsors/ismaeldamiao).

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
