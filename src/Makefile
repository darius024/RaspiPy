CC      ?= gcc
CFLAGS  ?= -std=c17 -g\
	-D_POSIX_SOURCE -D_DEFAULT_SOURCE\
	-Wall -Werror -pedantic

.SUFFIXES: .c .o

.PHONY: all clean

all: assemble emulate

assemble: assemble.o
decoders.o: decoders.c constants.h decoders.h instructions.h structs.h utils_em.h
emulate: emulate.o decoders.o io.o utils_em.o
emulate.o: emulate.c constants.h decoders.h instructions.h io.h structs.h utils_em.h
io.o: io.c io.h
utils_em.o: utils_em.c

clean:
	$(RM) *.o assemble emulate
	
