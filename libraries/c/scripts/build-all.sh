#!/usr/bin/env sh

sh "$(dirname "${0}")"/build.sh riscv32 linux
mkdir lib/riscv32
mv    lib/libc.so lib/riscv32/libc.so

sh "$(dirname "${0}")"/build.sh riscv64 linux
mkdir lib/riscv64
mv    lib/libc.so lib/riscv64/libc.so


sh "$(dirname "${0}")"/build.sh armv7 linux
mkdir lib/armv7
mv    lib/libc.so lib/armv7/libc.so

sh "$(dirname "${0}")"/build.sh aarch64 linux
mkdir lib/aarch64
mv    lib/libc.so lib/aarch64/libc.so


sh "$(dirname "${0}")"/build.sh i386 linux
mkdir lib/i386
mv    lib/libc.so lib/i386/libc.so

sh "$(dirname "${0}")"/build.sh x86_64 linux
mkdir lib/x86_64
mv    lib/libc.so lib/x86_64/libc.so
