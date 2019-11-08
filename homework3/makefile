# CFLAGS = -ggdb3 -std=c11 -Wall -Wunused-parameter -Wstrict-prototypes -Werror -Wextra -Wshadow
# CFLAGS += -fsanitize=signed-integer-overflow
# CFLAGS += -Wno-sign-compare -Wno-unused-parameter -Wno-unused-variable
# CFLAGS += -fsanitize=address -fsanitize=undefined

calc:calc.c
	gcc -o $@ $^ $(CFLAGS)

chase:chase.c ./graphics/bmp.c ./graphics/graphics.c ./graphics/vector.c ./graphics/vixel.c ./graphics/image_server.c ./physics/collision.c ./action_lib/action_lib.c
	gcc -o $@ $^ $(CFLAGS) -lm -lpthread -O3
