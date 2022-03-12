#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

char filozof[] = {'O','O','O','O','O'};
int vilica[] = {1, 1, 1, 1, 1};

pthread_mutex_t K;
pthread_cond_t red[5];

void ispisi_stanje(int n)
{
    printf("%c %c %c %c %c (%d)\n", filozof[0], filozof[1], filozof[2], filozof[3], filozof[4], n + 1);
}

void jesti(int n) // n je redni broj filozofa
{
    pthread_mutex_lock(&K);
    filozof[n] = 'o';
    while (vilica[n] == 0 || vilica[(n + 1) % 5] == 0)
    {
        pthread_cond_wait(&red[n], &K);
    }
    vilica[n] = vilica[(n + 1) % 5] = 0;
    filozof[n] = 'X';
    ispisi_stanje(n);
    pthread_mutex_unlock(&K);

    sleep(2); // jedi

    pthread_mutex_lock(&K);
    filozof[n] = 'O';
    vilica[n] = vilica[(n + 1) % 5] = 1;
    pthread_cond_signal(&red[(n - 1) % 5]);
    pthread_cond_signal(&red[(n + 1) % 5]);
    ispisi_stanje(n);
    pthread_mutex_unlock(&K);
}

void *filozofiraj(void *rbr)
{
    while (1)
    {
        sleep(rand() % 3 + 1); // misli
        int *n = rbr;
        jesti(*n);
    }
}

int main()
{
    int *BR;
    pthread_t *t;
    BR = malloc(5 * sizeof(int));
    t = malloc(5 * sizeof(pthread_t));
 
    pthread_mutex_init(&K, NULL);
    for (int i = 0; i < 5; i++)
        pthread_cond_init(&red[i], NULL);

    for (int i = 0; i < 5; i++)
    {
        BR[i] = i;
        if (pthread_create(&t[i], NULL, filozofiraj, &BR[i]))
        {
            fprintf(stderr, "Nesto je poslo po zlu\n");
            exit(1);
        }
    }
    for (int j = 0; j < 5; j++)
    {
        pthread_join(t[j], NULL);
    }
}