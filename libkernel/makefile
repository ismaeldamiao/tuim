linux: tmp/unix/linux.o
	@ mkdir -p lib
	$(LD) $(LDFLAGS) tmp/unix/linux.o --shared -o lib/libkernel.so


tmp/%.s: src/%.S
	@ mkdir -p $(dir $@)
	$(CPP) $(CPPFLAGS) $< -o $@

tmp/%.o: tmp/%.s
	$(ASM) $(ASMFLAGS) $< -o $@

clean:
	rm -Rf tmp
