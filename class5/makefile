CFLAGS = -ggdb3 -std=c11 -Wall -Wunused-parameter -Wstrict-prototypes -Werror -Wextra -Wshadow
CFLAGS += -fsanitize=signed-integer-overflow
CFLAGS += -Wno-sign-compare -Wno-unused-parameter -Wno-unused-variable
CFLAGS += -fsanitize=address -fsanitize=undefined

example1:example1.c
	gcc -o $@ $^ $(CFLAGS)

golomb:golomb.c
	gcc -o $@ $^ $(CFLAGS)

vector:vector.c
	gcc -o $@ $^ $(CFLAGS)
