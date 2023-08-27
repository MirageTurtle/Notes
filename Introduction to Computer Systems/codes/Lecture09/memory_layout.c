#include <stdio.h>
#include <stdlib.h>

char big_array[1L << 24];   /* 16 MB */
char huge_array[1L << 31];  /* 2 GB */

int global = 0;

int useless() { return 0; }

int main()
{
    void *p1, *p2, *p3, *p4;
    int local = 0;
    p1 = malloc(1L << 28);  /* 256 MB */
    p2 = malloc(1L << 8);   /* 256 B */
    p3 = malloc(1L << 32);  /* 4 GB */
    p4 = malloc(1L << 8);   /* 256 B */
    /* Print */
    printf("Address of local:\t\t%p\n", &local);
    // printf("Address of global:\t\t%p\n", &global);
    
    printf("Address of p1:\t\t\t%p\n", &p1);
    printf("Address of p2:\t\t\t%p\n", &p2);
    printf("Address of p3:\t\t\t%p\n", &p3);
    printf("Address of p4:\t\t\t%p\n", &p4);

    printf("Address of big_array:\t\t%p\n", &big_array);
    printf("Address of huge_array:\t\t%p\n", &huge_array);

    printf("Address of main:\t\t%p\n", main);
    printf("Address of useless:\t\t%p\n", useless);

    return 0;
}