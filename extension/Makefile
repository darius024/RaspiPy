CC      = gcc
CFLAGS  = -std=c17 -g\
	-D_POSIX_SOURCE -D_DEFAULT_SOURCE\
	-Wall -Werror -pedantic -MD
YFLAGS = -vd

.SUFFIXES: .c .o

.PHONY: all clean

# Object files
TEST_OBJS = parser.o lexer.o testASToptimiser.o ast.o ir.o optimise_ir.o utils_ir.o ast_to_ir.o ../src/io.o testAST.o print.o eval.o rpy.o
PARSER_OBJS = parser.o lexer.o ast.o ../src/io.o testAST.o print.o ast_to_ir.o utils_ir.o eval.o ir.o rpy.o optimise_ir.o
COMPILER_OBJS = parser.o lexer.o ir.o ast.o ast_to_ir.o utils_ir.o eval.o ../src/io.o print.o aot.o rpy.o optimise_ir.o

# Target executables
TARGET = test parse compile

# Dependency files generated
DEP = $(TEST_OBJS:.o=.d) $(PARSER_OBJS:.o=.d) $(COMPILER_OBJS:.o=.d)

# Auto-generated files for parser
PARSER_GENERATED = lexer.c lexer.h lex.yy.c lex.yy.d parser.c.output parser.c parser.h parser.output y.tab.c y.tab.h

# Executable files to generate by default
all: test parse compile



# Rule to build the test executable
test: $(TEST_OBJS)
	$(CC) $(CFLAGS) $(TEST_OBJS) -o test

# Rule to build the parser executable
parse: $(PARSER_OBJS)
	$(CC) $(CFLAGS) $(PARSER_OBJS) -o parse

# Rule to build the compiler executable
compile: $(COMPILER_OBJS)
	$(CC) $(CFLAGS) $(COMPILER_OBJS) -o compile

lexer.c lexer.h: lexer.l
	flex --header-file=lexer.h -o lexer.c lexer.l

parser.c parser.h: parser.y
	bison -d $(YFLAGS) -o parser.c parser.y


# Clean rule to remove generated files
clean:
	$(RM) $(TEST_OBJS) $(PARSER_OBJS) $(COMPILER_OBJS) $(TARGET) $(DEP) $(PARSER_GENERATED)
	
