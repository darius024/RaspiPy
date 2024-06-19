CC      = gcc
CFLAGS  = -std=c17 -g\
	-D_POSIX_SOURCE -D_DEFAULT_SOURCE\
	-Wall -Werror -pedantic -MD

.SUFFIXES: .c .o

.PHONY: all clean

# Object files
TEST_OBJS = testASToptimiser.o ast.o ir.o optimise_ir.o utils_ir.o

# Target executables
TARGET = test

# Dependency files generated
DEP = $(TEST_OBJS:.o=.d)

# Executable files to generate by default
all: $(TARGET)

# Rule to build the emulate executable
test: $(TEST_OBJS)
	$(CC) $(TEST_OBJS) -o test

# Clean rule to remove generated files
clean:
	$(RM) $(TEST_OBJS) $(TARGET) $(DEP)
