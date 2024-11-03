all :linux

COMMON_FLAGS = -Wall -Werror -Wextra -fno-builtin-printf -Wno-unused-function -fPIC -I.


linux:
	mkdir -p out
	mkdir -p tmp
	@echo building out/liboe.so with flags:
	@echo '    '$(COMMON_FLAGS)
	@gcc -shared $$(find src -name "*.c") -o out/liboe.so $(COMMON_FLAGS) -fPIC -DOEUF64
	@x='0'; \
	for i in $$(find src -name "*.c"); do \
		gcc -c $$i -o tmp/$$x''.o $(COMMON_FLAGS) -DOEUF64; \
		x=$$(($$x + 1)); \
	done
	@echo building out/liboe.a with flags:
	@echo '    '$(COMMON_FLAGS)
	@ar rcs out/liboe.a $$(find tmp -name "*.o")
	@echo all done

profan:
	olivine build.olv

test: linux
	@gcc -o test/test.o test/test.c out/liboe.a $(COMMON_FLAGS)
	@./test/test.o > test/output.out
	@if ! cmp -s test/test_output test/output.out; then \
	    echo "=======================================";  \
	    echo "Expected output:"; \
	    cat test/test_output ;\
		echo ;\
	    echo "======================================="; \
	    echo "Received output:"; \
	    cat test/output.out ;\
		echo ;\
	    exit 1 ;\
	fi

