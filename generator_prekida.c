#define _XOPEN_SOURCE
#define _XOPEN_SOURCE_EXTENDED

#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <time.h>

int pid=0;

void prekidna_rutina(int sig){
    kill(pid, SIGKILL);
    exit(0);
}

int main(int argc, char *argv[]){
    pid=atoi(argv[1]);
    sigset(SIGINT, prekidna_rutina);
    srand((unsigned)time (NULL));

    while(1){
        sleep(rand() % 3 + 3);
        int sig[] = {SIGHUP, SIGABRT, SIGQUIT, SIGTERM};
        int i = rand() % 4;
        kill(pid, sig[i]);
   }
   return 0;
}