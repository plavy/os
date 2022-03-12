#include <stdio.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <unistd.h>
#include <malloc.h>
#include <pthread.h>

int n; // brojDretvi
int m; // brojIteracija
int a = 0;

atomic_int *ULAZ, *BROJ;

atomic_int max(atomic_int arr[], int k)
{
    atomic_int maks = arr[0];
    for (int i = 0; i < n; i++)
    {
        if (arr[i] > maks)
            maks = arr[i];
    }
    return maks;
}

void udi_u_kriticni_odsjecak(int i)
{
    ULAZ[i] = 1;
    BROJ[i] = max(BROJ, n) + 1;
    ULAZ[i] = 0;

    for (int j = 0; j < n; j++)
    {

        while (ULAZ[j] != 0)
            ;
        while (BROJ[j] != 0 && (BROJ[j] < BROJ[i] || (BROJ[j] == BROJ[i] && j < i)))
            ;
    }
}

void izadi_iz_kriticnog_odsjecka(int i)
{
    BROJ[i] = 0;
}

void *dretva(void *rbr)
{
    int *d = rbr;
    for (int i = 1; i <= m; i++)
    {
        udi_u_kriticni_odsjecak(*d);
        a++;
        izadi_iz_kriticnog_odsjecka(*d);
    }
    return NULL;
}

int main(int argc, char *argv[])
{
    int *BR;
    pthread_t *t;

    if (argc < 3)
    {
        fprintf(stderr, "Koristenje: %s brojDretvi brojIteracija\n", argv[0]);
        exit(1);
    }

    n = atoi(argv[1]);
    m = atoi(argv[2]);

    ULAZ = malloc(sizeof(atomic_int) * n);
    BROJ = malloc(sizeof(atomic_int) * n);

    BR = malloc(n * sizeof(int));
    t = malloc(n * sizeof(pthread_t));

    for (int i = 0; i < n; i++)
    {
        BR[i] = i;
        if (pthread_create(&t[i], NULL, dretva, &BR[i]))
        {
            fprintf(stderr, "Nesto je poslo po zlu\n");
            exit(1);
        }
    }

    for (int j = 0; j < n; j++)
    {
        pthread_join(t[j], NULL);
    }

    printf("Glavni: a = %d\n", a);
    return 0;
}