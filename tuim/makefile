ARCH=riscv32
ENTRY=main
MACHINE=host

DEP-SOURCES=src/dep/fmap.c src/dep/aligned_alloc.c src/dep/mprotect.c
DEP-OBJECTS=$(DEP-SOURCES:src/%.c=tmp/%.o)

TUIM-SOURCES=src/cli/main.c $(wildcard src/api/*.c)
TUIM-OBJECTS=$(TUIM-SOURCES:src/%.c=tmp/%.o)

MACHINE-SOURCES=$(wildcard src/machine-$(MACHINE)/*.c) \
   $(wildcard src/api/machine-$(MACHINE)/tuim_*.c) \
	$(wildcard src/arch/$(ARCH)/api/machine-$(MACHINE)/tuim_*.c)
MACHINE-OBJECTS=$(MACHINE-SOURCES:src/%.c=tmp/%.o)

tuim: $(DEP-OBJECTS) $(MACHINE-OBJECTS) $(TUIM-OBJECTS)
	@ mkdir -p bin
	$(LD) $(LDFLAGS) -l c -e $(ENTRY) -o bin/tuim $(DEP-OBJECTS) $(MACHINE-OBJECTS) $(TUIM-OBJECTS)

tuim/arch.h: src/arch/$(ARCH)/include/tuim/arch.h
	@ mkdir -p include/tuim
	cp src/arch/$(ARCH)/include/tuim/arch.h include/tuim/arch.h

tuim.so: $(MACHINE-OBJECTS)
	@ mkdir -p lib
	$(LD) $(LDFLAGS) -l c --shared -o lib/tuim.so $(MACHINE-OBJECTS)

tmp/%.o: src/%.c
	@ mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -I include -I src/arch/$(ARCH)/include -o $@ $<

clean:
	rm -Rf tmp lib bin
