SRC = $(wildcard src/*.c src/*/*.c src/*/*/*.c)
OBJ = $(SRC:.c=.o)

CC = gcc
CF_FLAGS = -Wall -Werror -Wextra -fPIC -I.
LD_FLAGS =

NAME_SO = liboe.so
NAME_A = liboe.a

all: $(NAME_SO) $(NAME_A)

$(NAME_SO): $(OBJ)
	$(CC) $(LD_FLAGS) $^ -o $@

$(NAME_A): $(OBJ)
	ar rcs $@ $^

%.o: %.c
	$(CC) $(CF_FLAGS) -c $< -o $@

test: $(NAME_A)
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
