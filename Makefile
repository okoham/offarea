P = offarea
OBJECTS = offarea.o 
CFLAGS = -ggdb3 -O0 -Qunused-arguments -std=c11 -Wall -Werror \
         -Wextra -Wno-sign-compare -Wno-unused-parameter \
		 -Wno-unused-variable -Wshadow
LDLIBS = -lm
CC = clang

$(P): $(OBJECTS)
