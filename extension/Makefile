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
PARSER_OBJS = ast.o parser.o lexer.o ../src/io.o main.o

# Target executables
TARGET = test parse

# Dependency files generated
DEP = $(TEST_OBJS:.o=.d) $(PARSER_OBJS:.o=.d)

# Auto-generated files for parser
PARSER_GENERATED = lexer.c lexer.h lex.yy.c lex.yy.d parser.c.output parser.c parser.h parser.output y.tab.c y.tab.h

# Executable files to generate by default
all: parse test



# Rule to build the emulate executable
test: $(TEST_OBJS)
	$(CC) $(TEST_OBJS) -o test

# Rule to build the parser executable
parse: $(PARSER_OBJS)
	$(CC) $(PARSER_OBJS) -o parse

lexer.c lexer.h: lexer.l
	flex --header-file=lexer.h -o lexer.c lexer.l

parser.c parser.h: parser.y
	bison -d $(YFLAGS) -o parser.c parser.y


# Clean rule to remove generated files
clean:
	$(RM) $(TEST_OBJS) $(PARSER_OBJS) $(TARGET) $(DEP) $(PARSER_GENERATED) parser test
	
