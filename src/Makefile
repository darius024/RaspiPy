CC      = gcc
CFLAGS  = -std=c17 -g\
	-D_POSIX_SOURCE -D_DEFAULT_SOURCE\
	-Wall -Werror -pedantic

.SUFFIXES: .c .o

.PHONY: all clean

# Object files
EMULATE_OBJS = emulate.o decoders.o execute.o io.o structs.o utils_em.o
ASSEMBLE_OBJS = assemble.o datatypes_as.o decoders.o disassembler.o io.o onepass.o structs.o utils_as.o vector.o utils_em.o

all: emulate assemble

# Rule to build the emulate executable
emulate: $(EMULATE_OBJS)
	$(CC) $(EMULATE_OBJS) -o emulate

# Rule to build the assemble executable
assemble: $(ASSEMBLE_OBJS)
	$(CC) $(ASSEMBLE_OBJS) -o assemble

# Rules to build the object files
assemble.o:     constants.h datatypes_as.h decoders.h disassembler.h io.h onepass.h structs.h utils_as.h vector.h
datatypes_as.o: datatypes_as.h
decoders.o:     constants.h decoders.h instructions.h structs.h utils_em.h
disassembler.o: constants.h datatypes_as.h disassembler.h onepass.h structs.h utils_as.h vector.h
emulate.o:      constants.h datatypes_em.h decoders.h execute.h io.h structs.h utils_em.h
execute.o:      constants.h datatypes_em.h execute.h structs.h utils_em.h
io.o:   	io.h
onepass.o:      constants.h datatypes_as.h instructions.h onepass.h utils_as.h vector.h
structs.o:      structs.h
utils_as.o:     constants.h datatypes_as.h utils_as.h vector.h
utils_em.o:     utils_em.h
vector.o:       vector.h

# Pattern rule to compile .c files to .o files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean rule to remove generated files
clean:
	$(RM) $(EMULATE_OBJS) $(ASSEMBLE_OBJS) all
