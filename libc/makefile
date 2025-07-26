SOURCES=\
   $(wildcard src/*.S) $(wildcard src/**/*.S)\
   $(wildcard src/*.c) $(wildcard src/**/*.c)
OBJECTS=\
   $(patsubst src/%.S,tmp/%.o,$(filter src/%.S,$(SOURCES))) \
   $(patsubst src/%.c,tmp/%.o,$(filter src/%.c,$(SOURCES)))


libc.so: $(OBJECTS)
	@ mkdir -p lib
	$(LD) $(LDFLAGS) --shared -l kernel -o lib/libc.so $(OBJECTS)

libc.a: $(OBJECTS)
	@ mkdir -p lib
	$(AR) $(ARFLAGS) src lib/libc.a $(OBJECTS)

libc.o: $(OBJECTS)
	@ mkdir -p lib
	$(LD) $(LDFLAGS) --relocatable -o lib/libc.o $(OBJECTS)

tmp/%.s: src/%.S
	@ mkdir -p $(dir $@)
	$(CPP) $(CPPFLAGS) -o $@ $<

tmp/%.o: tmp/%.s
	$(ASM) $(ASMFLAGS) -o $@ $<

tmp/%.o: src/%.c
	@ mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -I include -o $@ $<

clean:
	rm -Rf tmp lib
