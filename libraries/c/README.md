
as -o puts.o linux/aarch32/puts.asm
cc -o strlen.o generic/string/strlen.c
ld --shared -o libc.so *.o
rm *.o
mv libc.so "${TUIM_HOME}/lib/libc.so"
