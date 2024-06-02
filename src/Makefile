CC      = gcc
CFLAGS  = -std=c17 -g\
	-D_POSIX_SOURCE -D_DEFAULT_SOURCE\
	-Wall -Werror -pedantic


LDFLAGS = -lm

SRC_FILES := assemble.c \
             disassembler.c \
             utils.c \
	     emulate.c 

RM = rm -f

# Object files
ASSEMBLE_OBJS = assemble.o disassembler.o utils.o vector.o
EMULATE_OBJS = emulate.o

# Target executables
EMULATE = emulate
ASSEMBLE = assemble

# Default target
.PHONY: all disassembler utils
all: $(EMULATE) $(ASSEMBLE)


# Rule to build the target executable file
# It uses the linker to combine all object files

$(ASSEMBLE): $(ASSEMBLE_OBJS)
	$(CC) $(ASSEMBLE_OBJS) -o $(ASSEMBLE) $(LDFLAGS)

# Rule to build the emulate executable
$(EMULATE): $(EMULATE_OBJS)
	$(CC) $(EMULATE_OBJS) -o $(EMULATE) $(LDFLAGS)

# Pattern rule to compile .c files to .o files
# This rule applies to any .c file to generate the corresponding .o file
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean rule to remove generated files
# This helps to clean up the directory by removing object files and the combined object file
.PHONY: clean
clean:
	$(RM) $(ASSEMBLE_OBJS) $(EMULATE_OBJS) $(ASSEMBLE) $(EMULATE)


