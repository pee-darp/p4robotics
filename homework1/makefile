CFLAGS = -ggdb3 -std=c11 -Wall -Wunused-parameter -Wstrict-prototypes -Werror -Wextra -Wshadow
CFLAGS += -fsanitize=signed-integer-overflow
CFLAGS += -Wno-sign-compare -Wno-unused-parameter -Wno-unused-variable
CFLAGS += -fsanitize=address -fsanitize=undefined

collision:collision.c
	gcc -o $@ $^ -std=c11 -g -lm

cryptogram:cryptogram.c
	gcc -o $@ $^ $(CFLAGS) -lm

cryptest:cryptest.c
	gcc -o $@ $^ -std=c11 -g
