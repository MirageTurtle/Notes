/* Echo Line */

#include <stdio.h>

void echo()
{
    char buf[4];    /* Way too small! */
    gets(buf);
    puts(buf);
}

void call_echo()
{
    echo();
}

int main()
{
    printf("Type a string: ");
    call_echo();
    return 0;
}