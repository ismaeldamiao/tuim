# Tuim's ELF Interpreter for Linux

## Download

For now Tuim is distributed only in source form.

You can can get it with:

```bash
git clone https://github.com/ismaeldamiao/tuim
cd tuim
```

## Building from source

Make sure you have:
- GNU Make
- A C compiler and linker

First navigate to the `tuim` directory and set up the environment.

```bash
cd tuim
. etc/profile.d/environment-clang.sh # if your compiler is LLVM CLang
. etc/profile.d/environment-gcc.sh   # if your compiler is GCC
```

Now make:

```bash
make tuim tuim/arch.h
```

### License

Software produced within the Tuim Project are licenced under the MIT licence.

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
