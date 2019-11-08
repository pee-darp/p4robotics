CFLAGS = -ggdb3 -std=c11 -Wall -Wunused-parameter -Wstrict-prototypes -Werror -Wextra -Wshadow
CFLAGS += -fsanitize=signed-integer-overflow
CFLAGS += -Wno-sign-compare -Wno-unused-parameter -Wno-unused-variable
CFLAGS += -fsanitize=address -fsanitize=undefined

hashtable:hashtable.c
	gcc -o $@ $^ $(CFLAGS)

test:test.c hashtable.c
	gcc -o $@ $^ $(CFLAGS)

test_file:test_file.c
	gcc -o $@ $^ $(CFLAGS)
