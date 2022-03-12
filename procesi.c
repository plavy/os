#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <sys/shm.h>

int n; // brojProcesa
int m; // brojIteracija
int *a = 0;
bool verbose = false;

void proces()
{
    for (int i = 1; i <= m; i++)
    {
        (*a)++;
    }
}

void proces_verbose(int rbr)
{
    for (int i = 1; i <= m; i++)
    {
        printf("Proces %2d: iteracija %2d\n", rbr, i);
        (*a)++;
    }
}

int main(int argc, char *argv[])
{

    if (argc < 3)
    {
        fprintf(stderr, "Koristenje: %s brojProcesa brojIteracija [-v]\n", argv[0]);
        exit(1);
    }

    n = atoi(argv[1]);
    m = atoi(argv[2]);
    if (argc == 4 && strcmp(argv[3], "-v") == 0)
        verbose = true;

    int shmId = shmget(IPC_PRIVATE, sizeof(int), 0660);
    if (shmId == -1)
    {
        fprintf(stderr, "Nesto je poslo po zlu\n");
        exit(1);
    }

    a = (int *)shmat(shmId, NULL, 0);

    if (verbose)
    {
        for (int i = 0; i < n; i++)
        {
            if (fork() == 0)
            {
                proces_verbose(i);
                exit(0);
            }
        }
    }
    else
    {
        for (int i = 0; i < n; i++)
        {
            if (fork() == 0)
            {
                proces(i);
                exit(0);
            }
        }
    }

    for (int j = 0; j < n; j++)
    {
        wait(NULL);
    }

    printf("Glavni: a = %d\n", *a);

    shmdt((int *)a);
    shmctl(shmId, IPC_RMID, NULL);

    return 0;
}