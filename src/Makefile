CC      ?= gcc
CFLAGS  ?= -std=c17 -g\
	-D_POSIX_SOURCE -D_DEFAULT_SOURCE\
	-Wall -Werror -pedantic
LDFLAGS ?= -lm

.SUFFIXES: .c .o

.PHONY: all clean

SRC_FILES := src/assemble.c \
             src/disassemblers/dataProcessing.c \
             src/disassemblers/loadAndStore.c \
             src/disassemblers/branching.c \
             src/disassemblers/directive.c \
             src/utils/assemble_utils.c

OBJ_FILES := $(SRC_FILES:.c=.o)

all: assemble emulate

assemble: assemble.o $(OBJ_FILES)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
emulate: emulate.o

clean:
	$(RM) *.o assemble emulate
	
