SOURCES=$(wildcard src/**/*.c)
OBJECTS=$(SOURCES:src/%.c=tmp/%.o)

libc.so: $(OBJECTS)
	@ mkdir -p lib
	$(LD) $(LDFLAGS) --shared -l kernel -o lib/libc.so $(OBJECTS)

libc.a: $(OBJECTS)
	@ mkdir -p lib
	$(AR) $(ARFLAGS) src lib/libc.a $(OBJECTS)

libc.o: $(OBJECTS)
	@ mkdir -p lib
	$(LD) $(LDFLAGS) --relocatable -o lib/libc.o $(OBJECTS)

tmp/%.o: src/%.c
	@ mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -I include -o $@ $<

clean:
	rm -Rf tmp lib
