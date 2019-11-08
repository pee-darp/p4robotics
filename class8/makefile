 CFLAGS = -ggdb3 -std=c11 -Wall -Wunused-parameter -Wstrict-prototypes -Werror -Wextra -Wshadow
 CFLAGS += -fsanitize=signed-integer-overflow
 CFLAGS += -Wno-sign-compare -Wno-unused-parameter -Wno-unused-variable
 CFLAGS += -fsanitize=address -fsanitize=undefined

bisect:bisect.c
	gcc -o $@ $^ $(CFLAGS)

binarysearch:binarysearch.c
	gcc -o $@ $^ -lm $(CFLAGS)

fibonacci:fibonacci.c
	gcc -o $@ $^ $(CFLAGS) -O3
