#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/shm.h>

sem_t *K;             // BSem
sem_t *djedbozicnjak; // BSem
sem_t *konzultacije;  // OSem
sem_t *sobovi;        // OSem

int *br_sobova = 0;    // broj sobova pred vratima
int *br_patuljaka = 0; // broj patuljaka pred vratima

void bozicnjak()
{
    while (1)
    {
        sleep(1);
        printf("Bozicnjak: Spavam...\n");
        sem_wait(djedbozicnjak);
        sem_wait(K);
        if (*br_sobova == 10 && *br_patuljaka > 0)
        {
            sem_post(K);

            printf("Bozicnjak: Raznosim poklone\n");
            sleep(2);
            printf("Bozicnjak: Raznio sam poklone\n");

            for (int i = 0; i < 10; i++)
            {
                sem_post(sobovi);
            }
        }
        else if (*br_sobova == 10)
        {
            sem_post(K);

            printf("Bozicnjak: Hranim sobove\n");
            sleep(2);
            printf("Bozicnjak: Nahranio sam sobove\n");
        }
        else
        {
            sem_post(K);
        }
        sem_wait(K);
        if(*br_patuljaka >= 3)
        {
            sem_post(K);

            printf("Bozicnjak: Rjesavam problem\n");
            sleep(2);
            printf("Bozicnjak: Rijesio sam problem\n");

            for (int i = 0; i < 3; i++)
            {
                sem_post(konzultacije);
            }
        } else {
            sem_post(K);
        }
    }
}

void patuljak()
{
    sem_wait(K);
    (*br_patuljaka)++;
    printf("Patuljak %d: Ja sam nov\n", *br_patuljaka);
    if((*br_patuljaka) % 3 == 0) {
        sem_post(djedbozicnjak);
    }
    sem_post(K);

    sem_wait(konzultacije);

    sem_wait(K);
    printf("Patuljak %d: Gotov sam s konzultacijama\n", *br_patuljaka);
    (*br_patuljaka)--;
    sem_post(K);
}

void sob()
{
    sem_wait(K);
    (*br_sobova)++;
    printf("Sob %d: Ja sam nov\n", *br_sobova);
    if((*br_sobova) >= 10) {
        sem_post(djedbozicnjak);
    }
    sem_post(K);

    sem_wait(sobovi);

    sem_wait(K);
    printf("Sob %d: Odlazim na odmor\n", *br_sobova);
    (*br_sobova)--;
    sem_post(K);
}

int main()
{
    int ID;
    ID = shmget(IPC_PRIVATE, 3 * sizeof(sem_t), 0600);
    K = (sem_t *)shmat(ID, NULL, 0);
    sem_init(K, 1, 1);
    djedbozicnjak = K + 1;
    sem_init(djedbozicnjak, 1, 0);
    konzultacije = djedbozicnjak + 1;
    sem_init(konzultacije, 1, 0);
    sobovi = konzultacije + 1;
    sem_init(sobovi, 1, 0);
    shmctl(ID, IPC_RMID, NULL);

    ID = shmget(IPC_PRIVATE, 2 * sizeof(int), 0600);
    br_sobova = (int *)shmat(ID, NULL, 0);
    br_patuljaka = br_sobova + 1;
    shmctl(ID, IPC_RMID, NULL);

    if (fork() == 0)
    {
        bozicnjak();
        exit(0);
    }

    while (1)
    {
        sleep(rand() % 3 + 1);
        int vjerojatnost_pojavljivanja_soba = rand() % 2;
        sem_wait(K);
        if (vjerojatnost_pojavljivanja_soba == 0 && *br_sobova < 10)
        {
            if (fork() == 0)
            {
                sob();
                exit(0);
            }
        }
        sem_post(K);
        int vjerojatnost_pojavljivanja_patuljka = rand() % 2;
        if (vjerojatnost_pojavljivanja_patuljka == 0)
        {
            if (fork() == 0)
            {
                patuljak();
                exit(0);
            }
        }
    }
}