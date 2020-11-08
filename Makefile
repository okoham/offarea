P = offarea
OBJECTS = offarea.o 
CFLAGS = -std=gnu11 -ggdb3 -O0 -Qunused-arguments \
		 -Wall -Werror \
         -Wextra -Wno-sign-compare -Wno-unused-parameter \
		 -Wno-unused-variable -Wshadow
LDLIBS = -lm
CC = clang

$(P): $(OBJECTS)

