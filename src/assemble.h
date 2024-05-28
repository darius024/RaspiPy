
#define MAX_TOKEN_LENGTH 20
#define MAX_INSTRS 200

struct symbolTable
{
    char label[MAX_TOKEN_LENGTH];
    int address;
};
extern struct symbolTable symtable[MAX_INSTRS];
extern int numLabel = 0;