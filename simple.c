#define _XOPEN_SOURCE
#define _XOPEN_SOURCE_EXTENDED

#include <stdio.h>
#include <signal.h>

void print_s(int s) {
    printf("Signal: %d\n", s);
}

int main () {
    printf("Proces obrade prekida, PID=%d\n", getpid());
    sigset(SIGTERM, print_s);
    sigset(SIGQUIT, print_s);

    while (1)
    {
        sleep(1);
    }
    return 0;
}