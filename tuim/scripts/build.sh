#!/usr/bin/env sh

# -------------------------------------
#  Script to build the command line interface.
#  * Part of Tuim project
#  * Last modified: June 20, 2025.
# -------------------------------------

[ -z "${POSIX_C_SOURCE}" ] && POSIX_C_SOURCE=200112L
[ -z "${CC}" ] && CC=cc
[ -z "${LD}" ] && LD=cc

[ -d "tmp" ] || mkdir tmp
[ -d "lib" ] || mkdir lib
[ -d "bin" ] || mkdir bin

cd tmp
"${CC}" ${CFLAGS} -c \
   -D _POSIX_C_SOURCE=${POSIX_C_SOURCE} \
   ../src/api/tuim_newcontext.c \
   ../src/api/tuim_deletecontext.c \
   ../src/api/tuim_errno.c \
   ../src/api/tuim_strerror.c \
   ../src/api/tuim_writeerror.c \
   ../src/api/tuim_getpath.c \
   ../src/api/tuim_loader.c \
   ../src/api/tuim_linker.c
"${CC}" ${CFLAGS} -c \
   -D _POSIX_C_SOURCE=${POSIX_C_SOURCE} \
   ../src/cli/main.c
cd ..

"${LD}" ${LDFLAGS} -o bin/tuim \
   tmp/tuim_newcontext.o \
   tmp/tuim_deletecontext.o \
   tmp/tuim_errno.o \
   tmp/tuim_strerror.o \
   tmp/tuim_writeerror.o \
   tmp/tuim_getpath.o \
   tmp/tuim_loader.o \
   tmp/tuim_linker.o \
   tmp/main.o
