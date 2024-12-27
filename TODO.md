## ABI and API

- [ ] Write CRT specifications processor that do not have it
- [ ] Choosen a library for systems interation (Actually using POSIX API)
- [ ] Choosen a library for graphical applications (propose: SDL)

## By software

* **LibTuim**
   - [x] Proccess loading
   - [x] Symbol finding
   - [x] Dependencies loading
   - [ ] Relocation (partially, see processors bellow)
   - [ ] Hash table (for now unnecessary to load and run all tests)
   - [ ] Initialization and Termination Functions (Will be implemented as necessary, with libc)
* **Loaders**
   - [ ] CLI
      - [x] Load and execute ELF images
      - [ ] Execute from app name instead of file image, finding on  default places
      - [ ] Execute by default (very OS dependent feature)
   - [ ] Android wrapper to CLI
* **C Standard Library**
   - [ ] ANSI C
      - [ ] `<assert.h>`
      - [ ] `<ctype.h>`
      - [ ] `<errno.h>`
      - [ ] `<float.h>`
      - [ ] `<limits.h>`
      - [ ] `<locale.h>`
      - [ ] `<math.h>`
      - [ ] `<setjmp.h>`
      - [ ] `<signal.h>`
      - [ ] `<stdarg.h>`
      - [ ] `<stddef.h>`
      - [ ] `<stdio.h>`
      - [ ] `<stdlib.h>`
      - [ ] `<string.h>`
      - [ ] `<time.h>`
   - [ ] C95
      - [x] `<iso646.h>`
      - [ ] `<wchar.h>`
      - [ ] `<wctype.h>`
   - [ ] C99
      - [ ] `<complex.h>`
      - [ ] `<fenv.h>`
      - [ ] `<inttypes.h>`
      - [ ] `<stdbool.h>`
      - [ ] `<stdint.h>`
      - [ ] `<tgmath.h>`
   - [ ] C11
      - [ ] `<stdalign.h>`
      - [ ] `<stdatomic.h>`
      - [ ] `<stdnoreturn.h>`
      - [ ] `<threads.h>`
      - [ ] `<uchar.h>`
   - [ ] C23
      - [ ] `<stdbit.h>`
      - [ ] `<stdckdint.h>`

## By processor

* **ARM**
   - [x] Rellocations (LibTuim)
   - [ ] AEABI (CRT)
* **AARCH64**
   - [ ] Rellocations (LibTuim)
   - [ ] CRT
* **i386**
   - [ ] Rellocations (LibTuim)
   - [ ] CRT
* **AMD 64**
   - [ ] Rellocations (LibTuim)
   - [ ] CRT
* **RISC-V**
   - [ ] Rellocations (LibTuim)
   - [ ] CRT

## By OS

* **Linux kernel**
   - [ ] C Standard Library
* **Windows**
   - [ ] C Standard Library
* **XNU**
   - [ ] C Standard Library
