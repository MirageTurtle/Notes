#include <stdio.h>
#include <stdlib.h>

#include "support.h"
#include "support_asm.h"

void win(int arg) {
    if (arg == 0x15213) {
        printf("You win 1 cookie! Great start!\n");
        return;
    }

    if (arg == 0x18213) {
        printf("You win 2 cookies! Woohoo!\n");
        return;
    }

    if (arg == 0x18613) {
        printf("You win 3 cookies! That's right!\n");
        return;
    }

    // "There is no secret activity!"
    if (mystery == 0x15513) {
        printf("You win 4 cookies! Outstanding!\n");
        return;
    }

    printf("You didn't win any cookies this time :(\n");
}

void solve(void) {
    volatile long before = 0xb4;
    char buf[16];
    volatile long after = 0xaf;

    Gets(buf);

    if (before == 0x3331323531) {
        win(0x15213);
    }

    if (after == 0x3331323831) {
        win(0x18213);
    }
}

int main(int argc, char *argv[]) {
    // Run solve() on a different stack
    run_on_stack(solve, stack_top);

    printf("Returning normally from main\n");
    return 0;
}
