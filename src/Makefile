CC      = gcc
CFLAGS  = -std=c17 -g\
	-D_POSIX_SOURCE -D_DEFAULT_SOURCE\
	-Wall -Werror -pedantic


LDFLAGS = -lm

SRC_FILES := assemble.c \
             disassembler.c \
             utils.c  
             # src/emulate.c 

RM = rm -f

# Object files
OBJ_FILES = $(SRC_FILES:.c=.o)

# Target executables
EMULATE = emulate
ASSEMBLE = assemble

# Default target
.PHONY: all
all: $(EMULATE) $(ASSEMBLE)


# Rule to build the target executable file
# It uses the linker to combine all object files
$(EMULATE): $(OBJ_FILES)
	$(CC) $(CFLAGS) $(OBJ_FILES) -o $(EMULATE)
	
$(ASSEMBLE): $(OBJ_FILES)
	$(CC) $(CFLAGS) $(OBJ_FILES) -o $(ASSEMBLE)

# Pattern rule to compile .c files to .o files
# This rule applies to any .c file to generate the corresponding .o file
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean rule to remove generated files
# This helps to clean up the directory by removing object files and the combined object file
.PHONY: clean
clean:
	$(RM) $(OBJ_FILES) $(ASSEMBLE) $(EMULATE)


