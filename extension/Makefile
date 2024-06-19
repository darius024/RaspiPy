CC      = gcc
CFLAGS  = -std=c17 -g\
	-D_POSIX_SOURCE -D_DEFAULT_SOURCE\
	-Wall -Werror -pedantic -MD
YFLAGS = -vd
LFLAGS = -l

.SUFFIXES: .c .o

.PHONY: all clean

# Object files
TEST_OBJS = testASToptimiser.o ast.o ir.o optimise_ir.o utils_ir.o
PARSER_OBJS = ast.o parser.o lexer.o main.o

# Target executables
TARGET = test parser

# Dependency files generated
DEP = $(TEST_OBJS:.o=.d) $(PARSER_OBJS:.o=.d)

# Executable files to generate by default
all: $(TARGET)

# Rule to build the emulate executable
test: $(TEST_OBJS)
	$(CC) $(TEST_OBJS) -o test

# Rule to build the parser executable
parser: $(PARSER_OBJS)
	$(CC) $(PARSER_OBJS) -o parser

lexer.c: lexer.l
	lex $(LFLAGS) -o lexer.c lexer.l

parser.c parser.h:       parser.y
	yacc $(YFLAGS) -o parser.c parser.y

# Clean rule to remove generated files
clean:
	$(RM) $(TEST_OBJS) $(PARSER_OBJS) $(TARGET) $(DEP) parser.c parser.h
