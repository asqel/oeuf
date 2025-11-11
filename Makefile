SRC = $(wildcard src/*.c src/*/*.c src/*/*/*.c)
OBJ = $(SRC:.c=.o)

CC = gcc
LD = gcc

CFLAGS = -Wall -Wextra -Iinclude -I. -g
LDFLAGS = -shared

NAME = liboeuf
NAME_SO = $(NAME).so
NAME_A = $(NAME).a

all: $(NAME_SO) $(NAME_A)

$(NAME_SO): $(OBJ)
	$(LD) $(LDFLAGS) $^ -o $@

$(NAME_A): $(OBJ)
	ar rcs $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME_A) $(NAME_SO)

re: fclean clean all

test:
	echo OK

.PHONY: re fclean clean all test

