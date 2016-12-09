#include "dynstack.h"
#include <stdio.h>
#include <stdlib.h>

void scope_stack_push(int number) {
    if(scope_stack_top == scope_stack_size-1) {
        scope_stack_resize();
    }

    scope_stack_top++;
    scope_stack[scope_stack_top] = number;
}

int scope_stack_pop() {
    if(scope_stack_top != -1) {
        int temp = scope_stack_top;
        scope_stack_top--;
        return scope_stack[temp];
    }
    else return -1;
}

void scope_stack_init() {
    scope_stack = malloc(sizeof(int) * INITIAL_SIZE);
    scope_stack_size = INITIAL_SIZE;
    scope_stack_top = -1;
}

void scope_stack_resize() {
    int *new_stack = malloc(sizeof(int) * scope_stack_size * 2);
    for(int i = 0; i <= scope_stack_top; i++) {
        new_stack[i] = scope_stack[i];
    }
    free(scope_stack);

    scope_stack = new_stack;

    scope_stack_size = scope_stack_size*2;
}

void print_stack() {
    for(int i = 0; i <= scope_stack_top; i++) printf("%d\n",scope_stack[i]);
}
