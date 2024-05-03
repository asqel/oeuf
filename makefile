all :linux

COMMON_FLAGS = -Wall -Werror -Wextra -fno-builtin-printf -Wno-unused-function -fPIC


linux:
	mkdir -p out
	mkdir -p tmp
	@echo building out/liboeuf.so with flags:
	@echo '    '$(COMMON_FLAGS)
	@gcc -shared $$(find src -name "*.c") -o out/liboeuf.so $(COMMON_FLAGS)
	@x='0'; \
	for i in $$(find src -name "*.c"); do \
		gcc -c $$i -o tmp/$$x''.o $(COMMON_FLAGS); \
		x=$$(($$x + 1)); \
	done
	@echo building out/liboeuf.a with flags:
	@echo '    '$(COMMON_FLAGS)
	@ar rcs out/liboeuf.a $$(find tmp -name "*.o")
	@echo all done

profan:
	olivine build.olv

test: linux
	gcc -o test.o test.c out/liboeuf.a $(COMMON_FLAGS)
	./test.o