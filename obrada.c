#define _XOPEN_SOURCE
#define _XOPEN_SOURCE_EXTENDED

#include <stdio.h>
#include <signal.h>

#define N 6 /* broj razina proriteta */

static int OZNAKA_CEKANJA[N];
static int PRIORITET[N];
static int TEKUCI_PRIORITET;

static int sig[] = {SIGHUP, SIGABRT, SIGQUIT, SIGTERM, SIGINT};

void zabrani_prekidanje()
{
    int i;
    for (i = 0; i < 5; i++)
        sighold(sig[i]);
}

void dozvoli_prekidanje()
{
    int i;
    for (i = 0; i < 5; i++)
        sigrelse(sig[i]);
}

void obrada_prekida(int x)
{
    switch (x)
    {
    case 1:
        printf("- P - - - -\n");
        for (int i = 1; i <= 5; i++)
        {
            printf("- %d - - - -\n", i);
            sleep(1);
        }
        printf("- K - - - -\n");
        break;
    case 2:
        printf("- - P - - -\n");
        for (int i = 1; i <= 5; i++)
        {
            printf("- - %d - - -\n", i);
            sleep(1);
        }
        printf("- - K - - -\n");
        break;
    case 3:
        printf("- - - P - -\n");
        for (int i = 1; i <= 5; i++)
        {
            printf("- - - %d - -\n", i);
            sleep(1);
        }
        printf("- - - K - -\n");
        break;
    case 4:
        printf("- - - - P -\n");
        for (int i = 1; i <= 5; i++)
        {
            printf("- - - - %d -\n", i);
            sleep(1);
        }
        printf("- - - - K -\n");
        break;
    case 5:
        printf("- - - - - P\n");
        for (int i = 1; i <= 5; i++)
        {
            printf("- - - - - %d\n", i);
            sleep(1);
        }
        printf("- - - - - K\n");
        break;
    }
}
void prekidna_rutina(int s)
{
    int n = -1;
    zabrani_prekidanje();
    switch (s)
    {
    case SIGHUP:
        n = 1;
        printf("- X - - - -\n");
        break;
    case SIGABRT:
        n = 2;
        printf("- - X - - -\n");
        break;
    case SIGQUIT:
        n = 3;
        printf("- - - X - -\n");
        break;
    case SIGTERM:
        n = 4;
        printf("- - - - X -\n");
        break;
    case SIGINT:
        n = 5;
        printf("- - - - - X\n");
        break;
    }
    OZNAKA_CEKANJA[n]++;
    int x;
    do
    {
        /* odredi signal najveceg prioriteta koji ceka na obradu */
        x = 0;
        for (int j = TEKUCI_PRIORITET + 1; j < N; j++)
        {
            if (OZNAKA_CEKANJA[j] != 0)
            {
                x = j;
            }
        }
        /* ako postoji signal koji ceka i prioritetniji je od trenutnog posla, idi u obradu */
        if (x > 0)
        {
            OZNAKA_CEKANJA[x]--;
            PRIORITET[x] = TEKUCI_PRIORITET;
            TEKUCI_PRIORITET = x;
            dozvoli_prekidanje();
            obrada_prekida(x);
            zabrani_prekidanje();
            TEKUCI_PRIORITET = PRIORITET[x];
        }

    } while (x > 0);
    dozvoli_prekidanje();
}

int main(void)
{
    sigset(sig[0], prekidna_rutina);
    sigset(sig[1], prekidna_rutina);
    sigset(sig[2], prekidna_rutina);
    sigset(sig[3], prekidna_rutina);
    sigset(sig[4], prekidna_rutina);

    printf("Proces obrade prekida, PID=%d\n", getpid());
    printf("G 1 2 3 4 5\n");

    /* troši vrijeme da se ima šta prekinuti - 10 s */
    for (int i = 1; i <= 10; i++)
    {
        printf("%d - - - - -\n", i);
        sleep(1);
    }

    printf("Zavrsio osnovni program\n");

    return 0;
}