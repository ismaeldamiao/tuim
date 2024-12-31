// sh scripts/tuim-cc -ffreestanding -nostdlib -I include -D __linux__ -o tmp/libc.o src/libc.c
// sh scripts/tuim-ld --shared -o lib/libc.so -lcrt tmp/libc.o
#include "ctype/ctype.c"
#include "stddef/stddef.c"
#include "stdio/stdio.c"
#include "string/string.c"
#include "math/math.c"
