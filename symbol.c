#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "symbol.h"
#include "dynstack.h"

int stack_ptr = -1;

Symbol* makeSymbol(int type, char* id, int scope) {
    Symbol* sym = (Symbol *)malloc(sizeof(Symbol));
    sym->type = type;
    strcpy(sym->id,id);
    sym->scope = scope;
    sym->deleted = 0;
    return sym;
}

void makeSymbolTable(int size) {
    st = (SymbolTable *)malloc(sizeof(SymbolTable));
    st->size = size;
    st->table = (Symbol **)malloc(sizeof(Symbol)*size);
    st->elements = 0;
    int i;
    for(i=0; i<size; i++) st->table[i] = NULL;
}

void printSymbolTable() {
    int i;
    for(i = 0; i<st->size; i++) if(st->table[i] != NULL) printf("Type: %d ID: %s Address: %d Scope: %d\n",st->table[i]->type,st->table[i]->id,st->table[i]->address,st->table[i]->scope);
}

void increaseSize() {
    SymbolTable *tempTable = st;
    makeSymbolTable(st->size*2);
    int i;
    for(i = 0; i<st->size; i++) {
        if(tempTable->table[i] != NULL && tempTable->table[i]->deleted == 0) insertSymbol(st->table[i]);
    }
    free(tempTable);
}

int insertSymbol(Symbol *symbol) {
    if((float) (st->elements/st->size) > 0.75) increaseSize();

    int start = hashFunction(symbol);
    int counter = 0;
    int done = 0;

    while(done == 0) {
        if(st->table[start] == NULL) {
            st->table[start] = symbol;
            st->elements++;
            done = 1;
        }
        else {
            start++;
            counter++;
            if(start >= st->size) start = 0;
        }
        if(counter == st->size && done == 0) {
            return 0;
        }
    }

    return 1;
}

int hashFunction(Symbol *symbol) {
    int i, sum = 0, limit = strlen(symbol->id);
    for(i=0; i<limit; i++) sum += symbol->id[i];
    sum = ((sum+symbol->scope)*31) % st->size;
    return sum;
}

int searchForSymbol(int type, char *id) {
    int found = -1;

    for(int i = scope_stack_top; i >= 0; i--) {
        found = exists(makeSymbol(type, id, scope_stack[i]));
        if(found != -1) break;
    }

    return found;
}

int exists(Symbol *symbol) {
    int start = hashFunction(symbol);
    int i;

    for(i = start; i<st->size; i++) {
        if(st->table[i]!=NULL) if(strcmp(st->table[i]->id,symbol->id) == 0 && st->table[i]->scope == symbol->scope && symbol->deleted == 0) return i;
    }
    for(i = 0; i<start; i++) if(st->table[i]!=NULL) if(strcmp(st->table[i]->id,symbol->id) == 0 && st->table[i]->scope == symbol->scope && symbol->deleted == 0) return i;

    return -1;
}

void delete(Symbol *symbol) {
    int spot = exists(symbol);
    if(spot != -1) {
        st->table[spot]->deleted = 1;
    }
}

Symbol *pop_symbol() {
    if(stack_ptr != -1) {
        stack_ptr--;
        return stack[stack_ptr+1];
    }
    else {
        printf("Stack Underflow.\n");
        return NULL;
    }
}

void push_symbol(Symbol *symbol) {
    stack_ptr++;
    stack[stack_ptr] = symbol;
}
