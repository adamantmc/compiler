#define INITIAL_SIZE 5

int scope_stack_size, scope_stack_top;
int *scope_stack;

void scope_stack_push(int number);
int scope_stack_pop();

void scope_stack_resize();
void scope_stack_init();
void print_stack();
