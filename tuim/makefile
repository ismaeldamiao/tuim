# export CC=clang
# export CFLAGS="-std=c23 -Wall -Wextra -Wpedantic -O3 -c"
# export LD=clang
# export LDFLAGS="-fuse-ld=lld"
# make

TUIM-SOURCES=src/cli/main.c \
   src/api/tuim_newcontext.c \
   src/api/tuim_deletecontext.c \
   src/api/tuim_errno.c \
   src/api/tuim_strerror.c \
   src/api/tuim_writeerror.c \
   src/api/tuim_getpath.c \
   src/api/tuim_loader.c \
   src/api/tuim_linker.c \
   src/api/tuim_exec.c
TUIM-OBJECTS=$(TUIM-SOURCES:src/%.c=tmp/%.o)

BACKEND-SOURCES=src/backend/host-machine/tuim_jump.c \
   src/backend/host-machine/tuim_aligned_alloc.c \
   src/backend/host-machine/tuim_free.c \
   src/backend/host-machine/tuim_load.c \
   src/backend/host-machine/tuim_malloc.c \
   src/backend/host-machine/tuim_memcpy.c \
   src/backend/host-machine/tuim_memset.c \
   src/backend/host-machine/tuim_mprotect.c \
   src/backend/host-machine/tuim_store.c \
   src/backend/tuim_get_dependency.c \
   src/backend/tuim_get_relocation.c \
   src/backend/tuim_load_segments.c \
   src/backend/tuim_attributes.c \
   src/backend/tuim_relocate.c \
   src/backend/tuim_get_symbol.c
BACKEND-OBJECTS=$(BACKEND-SOURCES:src/%.c=tmp/%.o)

tuim: $(BACKEND-OBJECTS) $(TUIM-OBJECTS)
	@ mkdir -p bin
	$(LD) $(LDFLAGS) -o bin/tuim $(BACKEND-OBJECTS) $(TUIM-OBJECTS)

backend: $(BACKEND-OBJECTS)
	@ mkdir -p lib
	$(LD) $(LDFLAGS) -o lib/tuim.so $(BACKEND-OBJECTS)

tmp/%.o: src/%.c
	@ mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -include src/tuim_build.h $< -o $@

clean:
	rm -Rf tmp
