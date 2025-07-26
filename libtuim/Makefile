MACHINE=host

DEP-SOURCES=src/dep/fmap.c
DEP-OBJECTS=$(DEP-SOURCES:src/%.c=tmp/%.o)

TUIM-SOURCES=$(wildcard src/*.c)
TUIM-OBJECTS=$(TUIM-SOURCES:src/%.c=tmp/%.o)

MACHINE-SOURCES=$(wildcard src/machine-$(MACHINE)/tuim_*.c)
MACHINE-OBJECTS=$(MACHINE-SOURCES:src/%.c=tmp/%.o)

libtuim.so: $(TUIM-OBJECTS) $(MACHINE-OBJECTS) $(DEP-OBJECTS)
	@ mkdir -p lib
	$(LD) $(LDFLAGS) -l c --shared -o lib/libtuim.so $(TUIM-OBJECTS) $(MACHINE-OBJECTS) $(DEP-OBJECTS)

libtuim.o: $(TUIM-OBJECTS) $(MACHINE-OBJECTS) $(DEP-OBJECTS)
	@ mkdir -p lib
	$(LD) $(LDFLAGS) -r -o  lib/libtuim.o $(TUIM-OBJECTS) $(MACHINE-OBJECTS) $(DEP-OBJECTS)

tuim.so: $(MACHINE-OBJECTS)
	@ mkdir -p lib
	$(LD) $(LDFLAGS) -l c --shared -o lib/tuim.so $(MACHINE-OBJECTS)

tmp/%.o: src/%.c
	@ mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -I include -o $@ $<

clean:
	rm -Rf tmp lib
