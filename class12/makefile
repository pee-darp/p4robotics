# CFLAGS = -ggdb3 -std=c11 -Wall -Wunused-parameter -Wstrict-prototypes -Werror -Wextra -Wshadow
# CFLAGS += -fsanitize=signed-integer-overflow
# CFLAGS += -Wno-sign-compare -Wno-unused-parameter -Wno-unused-variable
# CFLAGS += -fsanitize=address -fsanitize=undefined

hash:hash.c
	gcc -o $@ $^ $(CFLAGS)

hashcomp:hashcomp.c
	gcc -o $@ $^ $(CFLAGS) -O3
