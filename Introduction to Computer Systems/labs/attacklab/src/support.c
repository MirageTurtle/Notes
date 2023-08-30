#include <stdio.h>
#include <stdlib.h>

#include "support.h"

volatile int mystery = 0;

static char my_stack[STACK_SIZE] __attribute__ ((aligned (16)));
void *stack_top = my_stack + STACK_SIZE - 16;

void Gets(char *buf) {
    if (gets(buf) == NULL) {
        fprintf(stderr, "gets() error\n");
        exit(1);
    }
}
