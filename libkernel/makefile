KERNEL=linux
SOURCES=$(wildcard src/$(KERNEL)/sys_*.S)
OBJECTS=$(SOURCES:src/%.S=tmp/%.o)

all: tuim/kernel.h libkernel.so

tuim/kernel.h: src/$(KERNEL)/kernel.h
	@ mkdir -p include/tuim
	cp src/$(KERNEL)/kernel.h include/tuim/kernel.h

libkernel.so: $(OBJECTS)
	@ mkdir -p lib
	$(LD) $(LDFLAGS) --shared -o lib/libkernel.so $(OBJECTS)

libkernel.a: $(OBJECTS)
	@ mkdir -p lib
	$(AR) $(ARFLAGS) src lib/libkernel.a $(OBJECTS)

libkernel.o: $(OBJECTS)
	@ mkdir -p lib
	$(LD) $(LDFLAGS) --relocatable -o lib/libkernel.o $(OBJECTS)

tmp/%.s: src/%.S
	@ mkdir -p $(dir $@)
	$(CPP) $(CPPFLAGS) -o $@ $<

tmp/%.o: tmp/%.s
	$(ASM) $(ASMFLAGS) -o $@ $<

clean:
	rm -Rf tmp lib include
