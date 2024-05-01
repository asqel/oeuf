all :linux


linux:
	gcc -shared $$(find src -name "*.c") -o out/liboeuf.so
	x='0'; \
	for i in $$(find src -name "*.c"); do \
		gcc -c $$i -o tmp/$$x''.o; \
		x=$$(($$x + 1)); \
	done
	ar rcs out/liboeuf.a $$(find tmp -name "*.o")

profan:
	olivine build.olv

test: linux
	gcc -o test.o test.c out/liboeuf.a
	./test.o