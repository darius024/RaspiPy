

#include "ast.h"
#include "ir.h"
#include "optimise_ir.h"
#include <stdbool.h>
#include <inttypes.h>


#define INT_MAX_CHARS 20 // max number of digits int64_t can take

#define MAX_NAME_CHARS 20
#define VARS_NUM 3
#define CONST_FOLD_TESTS_NUM 2
#define CONST_PROP_TESTS_NUM 2


typedef bool (*comparator) (void *actual, void *expected);

static char mul = '*';
static char minus = '-';

/*
bool same_Expression(Expression e1, Expression e2) {
    if (e1 -> tag != e2 -> tag) {
        printf("Cannot compare different Expression types\n");
        exit(EXIT_FAILURE);
    }
    
    switch(tag) {
        case EXPR_NAME:
            return strcmp(e1 -> name, e2 -> name);
        case EXPR_INT:
            return (e1 -> int_value == e2 -> int_value); 
        case EXPR_BINARY_OP {
            BinaryOp b1 = e1 -> binary_op;
            BinaryOp b2 = e2 -> binary_op;
            return ((b1 -> op == b2 -> op)
                    && same_Expression(b1 -> left, e2 -> b2 -> left)
                    && same_Expression(b1 -> right, b2 -> right));
        }
        case EXPR_UNARY_OP {
            UnaryOp u1 = e1 -> unary_op;
            UnaryOp u2 = e2 -> unary_op;
            return ((u1 -> op == u2 -> op)
                    && same_Expression(u1 -> expression, u2 -> expression));
        }
        case EXPR_FUNCTION_CALL {
            FunctionCall fc1 = e1 -> function_call;
            FunctionCall fc2 = e2 -> function_call;
            if (fc1.arg_count != fc2.arg_count) {
                return false;
            }
            for (int i = 0 ; i < fc1.arg_count ; i++) {
                if (!same_Expression((fc1 -> args)[i], (fc2 -> args)[i])) {
                    return false;
                }
            }
            return true;
        }
    }
}
*/

char *get_Expression_str(Expression e) {
    char *str;
    switch(e.tag) {
        case EXPR_NAME:
            // e.name is a pointer to Name, which is a struct containing char *name
            str = malloc((strlen(e.name -> name) + 1) * sizeof(char));
            if (str == NULL) {
                exit(EXIT_FAILURE);
            }
            strcpy(str, e.name -> name);
            break;
        case EXPR_INT: {
            str = malloc(INT_MAX_CHARS * sizeof(char));
            if (str == NULL) {
                exit(EXIT_FAILURE);
            }
            snprintf(str, INT_MAX_CHARS * sizeof(char), "%" PRId64, e.int_value -> value);
            break;
        }
        case EXPR_BINARY_OP: {
            BinaryOp *b = e.binary_op;
            char *s1 = get_Expression_str(*(b -> left));
            char *s2 = get_Expression_str(*(b -> right));
            str = malloc(strlen(s1) + strlen(s2) + strlen(b -> op) + 5);
            
            // combining strings
            char *p = str;

            *p = '(';
            p++;
            strcpy(p, s1);
            p += strlen(s1);
            *p = ')';
            p++;

            strcpy(p, b -> op);
            p += strlen(b -> op);

            *p = '(';
            p++;
            strcpy(p, s2);
            p += strlen(s2);
            *p = ')';
            p++;
            *p = '\0';
            // freeing
            free(s1);
            free(s2);
            break;
        }
        case EXPR_UNARY_OP: {
            UnaryOp *u1 = e.unary_op;
            char *s1 = get_Expression_str(*(u1 -> expression));
            str = malloc(strlen(s1) + strlen(u1 -> op) + 3);
            
            // combining
            char *p = str;
            strcpy(p, u1 -> op);
            p += strlen(u1 -> op);
            *p++ = '(';
            strcpy(p, s1);
            *p++ = ')';
            *p = '\n';
    
            // freeing
            free(s1);
            break;
        }
        case EXPR_FUNCTION_CALL: {
            FunctionCall *fc = e.function_call;
            int arg_num = fc -> arg_count;
            char **arg_exprs = malloc(arg_num * sizeof(char *));
            int params_size = 0;

            char *arg_str;
            int i = 0;
            for (Arguments *args = fc -> args ; args != NULL ; args = args -> next) {
                arg_str = get_Expression_str(*(args -> arg));
                arg_exprs[i] = arg_str;
                params_size += strlen(arg_str);
                i++;
            }
            str = malloc((strlen(fc -> name) + 2 + params_size + 2 * ((arg_num >= 1) ? arg_num - 1 : 0)) * sizeof(char) + 1);
            
            // combining (with freeing)
            char *p = str;
            strcpy(p, fc -> name);
            p += strlen(fc -> name);
            *p = '(';
            p += 1;

            for (int i = 0 ; i < arg_num ; i++) {
                strcpy(p, arg_exprs[i]);
                p += strlen(arg_exprs[i]);
                strcpy(p, ", ");

                free(arg_exprs[i]);
                p += 2;
            }
            p -= 2; // backtrack to get rid of last ", "
            *p = ')';
            free(arg_exprs);
            break;
         }
    }
    return str;
}

bool same_representation(Expression e1, Expression e2) {
    return (strcmp(get_Expression_str(e1), get_Expression_str(e2)));
}

Entry createEntry(char *var, int64_t value) { // not stored on heap
    Entry entry;
    strcpy(entry.name, var);
    entry.value = value;
    return entry;
}

void addVarValueToState(char *var, int64_t value, int pos, State *state) {
    Entry e = createEntry(var, value);
    state -> map[pos] = e;
}

void test_const_fold() { 
    printf("Running const_fol()....\n");

    // initialising 

    State state;

    // setting up variables    
    
    char vars[VARS_NUM][MAX_NAME_CHARS] = {"a", "B", "a2"};
    int64_t values[VARS_NUM] = {4, 10, -5};
    Expression *varExpres[VARS_NUM];
    for (int i = 0 ; i < VARS_NUM ; i++) {
        varExpres[i] = create_expression(EXPR_NAME, create_name(vars[i]));
    }

    // filling state
    state.map_size = VARS_NUM;
    for (int i = 0 ; i < VARS_NUM ; i++) {
        addVarValueToState(vars[i], values[i], i, &state);        
    }

    // creating test inputs (mathematical expressions)
    Expression *testingExpres[CONST_FOLD_TESTS_NUM];
    // (a * 3) - a2
    testingExpres[0] = create_expression(EXPR_BINARY_OP,
        create_binary_op(&minus,
            create_expression(EXPR_BINARY_OP,
                create_binary_op(&mul, 
                    varExpres[0], create_expression(EXPR_INT, create_int(3)))), varExpres[2]));
    // a - B
    testingExpres[1] = create_expression(EXPR_BINARY_OP,
        create_binary_op(&minus, 
            varExpres[0], varExpres[1]));


    // creating expected outputs
    Expression *expectedExpres[CONST_FOLD_TESTS_NUM];
    // 17
    expectedExpres[0] = create_expression(EXPR_INT, create_int(17));
    // -6
    expectedExpres[1] = create_expression(EXPR_INT, create_int(-6));

    // creating actual oututs
    Expression *actualExpres[CONST_FOLD_TESTS_NUM];
    for (int i = 0 ; i < CONST_FOLD_TESTS_NUM ; i++) {
        actualExpres[i] = const_fold(testingExpres[i], &state);
    }

    // comparing
    for (int i = 0 ; i < CONST_FOLD_TESTS_NUM ; i++) {
        if (strcmp(
            get_Expression_str(*actualExpres[i]),
            get_Expression_str(*expectedExpres[i])) == 0) {
                printf("Test %d OK\n", i);
            }
        else {
            printf("Test %d FAIL\n", i);
            printf("   Expected %s\n", get_Expression_str(*expectedExpres[i]));
            printf("   Got %s\n", get_Expression_str(*actualExpres[i]));
        }
    }   
}

void test_const_prop() { 
    printf("Running const_prop()....\n");

    // initialising 

    State state;

    // setting up variables    
    
    char vars[VARS_NUM][MAX_NAME_CHARS] = {"a", "B", "a2"};
    int64_t values[VARS_NUM] = {4, 10, -5};
    Expression *varExpres[VARS_NUM];
    Expression *valueExpres[VARS_NUM];
    for (int i = 0 ; i < VARS_NUM ; i++) {
        varExpres[i] = create_expression(EXPR_NAME, create_name(vars[i]));
        valueExpres[i] = create_expression(EXPR_INT, create_int(values[i]));
    }

    // filling state
    state.map_size = VARS_NUM;
    for (int i = 0 ; i < VARS_NUM ; i++) {
        addVarValueToState(vars[i], values[i], i, &state);        
    }

    // creating test inputs (mathematical expressions)
    Expression *testingExpres[CONST_PROP_TESTS_NUM];
    // (a * B) - a2
    testingExpres[0] = create_expression(EXPR_BINARY_OP,
        create_binary_op(&minus,
            create_expression(EXPR_BINARY_OP,
                create_binary_op(&mul, 
                    varExpres[0], varExpres[1])), varExpres[2]));
    // a - B
    testingExpres[1] = create_expression(EXPR_BINARY_OP,
        create_binary_op(&minus, 
            varExpres[0], varExpres[1]));


    // creating expetected outputs
    Expression *expectedExpres[CONST_PROP_TESTS_NUM];
    expectedExpres[0] = create_expression(EXPR_BINARY_OP,
        create_binary_op(&minus,
            create_expression(EXPR_BINARY_OP,
                create_binary_op(&mul, 
                    valueExpres[0], valueExpres[1])), valueExpres[2]));
    expectedExpres[1] = create_expression(EXPR_BINARY_OP,
        create_binary_op(&minus, 
            valueExpres[0], valueExpres[1]));

    // creating actual oututs
    Expression *actualExpres[CONST_PROP_TESTS_NUM];
    for (int i = 0 ; i < CONST_PROP_TESTS_NUM ; i++) {
        actualExpres[i] = const_prop(testingExpres[i], &state);
    }

    // comparing
    for (int i = 0 ; i < CONST_PROP_TESTS_NUM ; i++) {
        if (strcmp(
            get_Expression_str(*actualExpres[i]),
            get_Expression_str(*expectedExpres[i])) == 0) {
                printf("Test %d OK\n", i);
            }
        else {
            printf("Test %d FAIL\n", i);
            printf("   Expected %s\n", get_Expression_str(*expectedExpres[i]));
            printf("   Got %s\n", get_Expression_str(*actualExpres[i]));
        }
    }   
}

int main(void) {
    test_const_prop();
    test_const_fold();
    return 0;
}

