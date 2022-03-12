#define _XOPEN_SOURCE
#define _XOPEN_SOURCE_EXTENDED

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/time.h>
#include <math.h>

int pauza = 0;
unsigned long broj = 1000000001;
unsigned long zadnji = 1000000001;

void periodicki_ispis(int sig)
{
    printf("%lu\n", zadnji);
}

void postavi_pauzu()
{
    pauza = 1 - pauza;
}

void prekini()
{
    printf("[SIGTERM] %lu\n", zadnji);
    exit(0);
}

int prost(unsigned long n)
{
    unsigned long i, max;

    if ((n & 1) == 0)
        return 0;

    max = sqrt(n);
    for (i = 3; i <= max; i += 2)
    {
        if ((n % i) == 0)
        {
            return 0;
        }
    }

    return 1; // prost
}

int main()
{
    sigset(SIGTERM, prekini);
    sigset(SIGINT, postavi_pauzu);

    struct itimerval t;
    sigset(SIGALRM, periodicki_ispis);
    t.it_value.tv_sec = 5;
    t.it_value.tv_usec = 0;
    t.it_interval.tv_sec = 5;
    t.it_interval.tv_usec = 0;
    setitimer(ITIMER_REAL, &t, NULL);

    while (1)
    {
        if (prost(broj) == 1)
            zadnji = broj;
        broj++;
        while (pauza == 1)
            pause();
    }
}