SOURCES=src/stdio/puts.c\
   src/string/strlen.c
UNIX-OBJECTS=$(SOURCES:src/%.c=tmp/unix-%.o)

unix: $(UNIX-OBJECTS)
	@ mkdir -p lib
	$(LD) $(LDFLAGS) $(UNIX-OBJECTS) --shared -l kernel -o lib/libc.so

tmp/unix-%.o: src/%.c
	@ mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -I include -D __unix__=1 $< -o $@

clean:
	rm -Rf tmp
