# Compatibility

This chapter describes the conventions assumed for
C Standard Library implementation made by I.F.F. dos Santos.
All compilers that uses the specified convenctions are teoreticaly
compatible and the headers can be used instead of systems's headers.

## Introduction

The public header files are the headers defined on standards,
these headers are intented to be appropriate for use with common predefined macros,
these macros are listed bellow.

* Architecture:
   1. `__aarch64__`: for ARM64 processors.
   1. `__x86_64__`: for x86-64 processors.
   1. `__java__`: for the Java Virtual Machine.
   1. `__arm__`: for ARM processors.
   1. `__i386__`: for x86 processors.
* Operational systems:
   1. `__ANDROID__`: for Android.
   1. `_WIN64`: for Windows 64 bit.
   1. `_WIN32`: for Windows 32 bit.
   1. `_WIN16`: for Windows 16 bit.
   1. `__linux__`: for OS that uses the Linux kernel.
   1. `__unix__`: for Unix like OS.

This documentation call _64 bit system_ all compilers
that pre-define any of the following macros:

* `__aarch64__`
* `__x86_64__`

This documentation call _32 bit system_ all compilers
that pre-define any of the following macros:

* `__java__`
* `__arm__`
* `__i386__`

## Integers types

On all 64 and 32 bit systems is assumed that
integers are represented using two's complement and that

* `char` is 8 bit,
* `short` is 16 bit,
* `int` is 32 bit and
* `long long` is 64 bit.

Also, on all 32 bit systems is assumed that `long` is 32 bit.
On all 64 bit systems is assumed that `long` is 64 bit iff `_WIN64`
is not defined, otherwise `long` is 32 bit.

## Floating point environment

On all 32 and 64 bit systems is assumed that
`float` and `double` are represented using IEEE 754 standard.

## Compilers

When the compiler pre-defines the macro `__GNUC__`
is assumed that GNU C compiler extensions are available.

* Microsoft C/C++ compiler.
* On LLVM CLang...
