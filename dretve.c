#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <malloc.h>
#include <string.h>
#include <pthread.h>

int n; // brojDretvi
int m; // brojIteracija
int a = 0;
bool verbose = false;

void *dretva()
{
    for (int i = 1; i <= m; i++)
    {
        a++;
    }
    return NULL;
}

void *dretva_verbose(void *rbr)
{
    int *d = rbr;
    for (int i = 1; i <= m; i++)
    {
        a++;
        printf("Dretva %2d: iteracija %2d\n", *d, i);
    }
    return NULL;
}

int main(int argc, char *argv[])
{
    int *BR;
    pthread_t *t;

    if (argc < 3)
    {
        fprintf(stderr, "Koristenje: %s brojDretvi brojIteracija [-v]\n", argv[0]);
        exit(1);
    }

    n = atoi(argv[1]);
    m = atoi(argv[2]);
    if (argc == 4 && strcmp(argv[3], "-v") == 0)
        verbose = true;

    BR = malloc(n * sizeof(int));
    t = malloc(n * sizeof(pthread_t));

    if (verbose)
    {
        for (int i = 0; i < n; i++)
        {
            BR[i] = i;
            if (pthread_create(&t[i], NULL, dretva_verbose, &BR[i]))
            {
                fprintf(stderr, "Nesto je poslo po zlu\n");
                exit(1);
            }
        }
    }
    else
    {
        for (int i = 0; i < n; i++)
        {
            if (pthread_create(&t[i], NULL, dretva, NULL))
            {
                fprintf(stderr, "Nesto je poslo po zlu\n");
                exit(1);
            }
        }
    }

    for (int j = 0; j < n; j++)
    {
        pthread_join(t[j], NULL);
    }

    printf("Glavni: a = %d\n", a);
    return 0;
}