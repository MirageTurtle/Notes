#ifndef SUPPORT_H
#define SUPPORT_H

#define STACK_SIZE 4096

extern volatile int mystery;
extern void *stack_top;

void Gets(char *buf);

#endif /* SUPPORT_H */
