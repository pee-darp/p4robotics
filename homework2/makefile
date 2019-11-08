CFLAGS = -ggdb3 -std=c11 -Wall -Wunused-parameter -Wstrict-prototypes -Werror -Wextra -Wshadow
CFLAGS += -fsanitize=signed-integer-overflow
CFLAGS += -Wno-sign-compare -Wno-unused-parameter -Wno-unused-variable
CFLAGS += -fsanitize=address -fsanitize=undefined

rasterize:rasterize.c bmp.c image_server.c vector.c bres_line.c test_cases.c
	gcc -o $@ $^ $(CFLAGS) -lpthread -lm

bres_line:bres_line.c vector.c
	gcc -o $@ $^ $(CFLAGS) -lm

braitenberg:braitenberg.c bmp.c bres_line.c collision.c geometry.c image_server.c vector.c
	gcc -o $@ $^ $(CFLAGS) -lm -lpthread
