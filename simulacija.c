#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>

int *indeksi;
char *prostor;
int n;
char trenutni = 'a';

void print()
{
    for (int i = 0; i < 80; i++)
    {
        printf("\n");
    }
    for (int i = 0; i < n; i++)
    {
        printf("%d", indeksi[i]);
    }
    printf("\n");
    for (int i = 0; i < n; i++)
    {
        printf("%c", prostor[i]);
    }
    printf("\n\n");
}

void zauzmi()
{
    print();
    int velicina = rand() % 5 + 1;
    printf("Zauzimam %c veličine %d", trenutni, velicina);
    for (int i = 0; i < 3; i++)
    {
        fflush(stdout);
        usleep(400000);
        printf(".");
    }
    fflush(stdout);
    usleep(400000);
    int indeks_najmanja_dostupna_rupa = -1;
    int velicina_najmanja_dostupna_rupa = n + 1;
    int indeks_rupa = 0;
    bool rupa = false;
    for (int i = 0; i < n; i++)
    {
        if (prostor[i] == '-' && rupa == false)
        {
            indeks_rupa = i;
            rupa = true;
        }
        if ((prostor[i] != '-' || i == n - 1) && rupa == true)
        {
            if (i == n - 1)
                i++;
            if (i - indeks_rupa >= velicina && i - indeks_rupa < velicina_najmanja_dostupna_rupa)
            {
                indeks_najmanja_dostupna_rupa = indeks_rupa;
                velicina_najmanja_dostupna_rupa = i - indeks_rupa;
            }
            if (i == n)
                i--;
            rupa = false;
        }
    }
    if (indeks_najmanja_dostupna_rupa == -1)
    {
        printf("\033[0;31m");
        print();
        printf("Nema dostupnog mjesta!");
        printf("\033[0m");
        fflush(stdout);
        usleep(500000);

        print();
        printf("Nema dostupnog mjesta!");
        fflush(stdout);
        usleep(500000);

        printf("\033[0;31m");
        print();
        printf("Nema dostupnog mjesta!");
        printf("\033[0m");
        fflush(stdout);
        usleep(500000);
        return;
    }

    for (int i = 0; i < velicina; i++)
    {
        prostor[i + indeks_najmanja_dostupna_rupa] = trenutni;
    }
    trenutni++;
}

void oslobodi(char a)
{
    print();
    for (int i = 0; i < n; i++)
    {
        if (prostor[i] == a)
        {
            prostor[i] = '-';
            print();
            printf("Oslobađam %c", a);
            fflush(stdout);
            usleep(200000);
        }
    }
}

void defragmentiraj()
{
    int pocetak_rupe = 0;
    for (int i = 0; i < n; i++)
    {
        if (prostor[i] == '-')
        {
            for (int j = i + 1; j < n; j++)
            {
                if (prostor[j] != '-')
                {
                    prostor[i] = prostor[j];
                    prostor[j] = '-';
                    printf("\033[0;33m");
                    print();
                    printf("Defragmentiram");
                    printf("\033[0m");
                    fflush(stdout);
                    usleep(400000);
                    break;
                }
            }
        }
    }
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Molim zadajte početnu veličinu.\n");
        exit(0);
    }
    n = atoi(argv[1]);
    indeksi = malloc(sizeof(int) * n);
    for (int i = 0; i < n; i++)
    {
        indeksi[i] = i % 10;
    }
    prostor = malloc(sizeof(char) * n);
    for (int i = 0; i < n; i++)
    {
        prostor[i] = '-';
    }
    char ulaz;
    while (1)
    {
        print();
        scanf("%s", &ulaz);
        if (ulaz == 'z')
        {
            zauzmi();
        }
        else if (ulaz == 'o')
        {
            print();
            printf("Oslobodi: ");
            char a;
            scanf("%s", &a);
            oslobodi(a);
        }
        else if (ulaz == 'd')
        {
            defragmentiraj();
        }
    }
}