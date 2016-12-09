#ifndef _SYMBOL_H
#define _SYMBOL_H

#define HASH_DEF_SIZE 10
#define INTEGER_TYPE 2373
#define INTEGER_ARR_TYPE 2374
#define INT_CONSTANT 2375
#define EXPRESSION 2376
#define EXP_L 2377
#define EXP_LEQ 2378
#define EXP_G 2379
#define EXP_GEQ 2380
#define EXP_EQ 2381
#define EXP_NEQ 2382
#define STORED_IN_MEM 2383
#define STORED_IN_RA 2384
#define STORED_IN_RX 2385

typedef struct _Symbol Symbol;
typedef struct _SymbolTable SymbolTable;

struct _Symbol {
    int type;
    int exp_type;
    int value;
    char id[256];

    int scope;
    char deleted;

    int storedIn;

    int address;
    int arr_dim;
};

struct _SymbolTable {
    Symbol **table;
    int size;
    int elements;
};

Symbol *stack[1024];
SymbolTable *st;


Symbol* makeSymbol(int type, char* id, int scope);

void makeSymbolTable(int size);

void printSymbolTable();

void increaseSize();

int insertSymbol(Symbol *symbol);

int hashFunction(Symbol *symbol);

int exists(Symbol *symbol);

int searchForSymbol(int type, char *id);

void push_symbol(Symbol *symbol);

Symbol *pop_symbol();

#endif
