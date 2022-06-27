
#include <bits/types/FILE.h>
#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include <signal.h>

void process_event(int sig);
void process_sigterm(int sig);
void process_sigint(int sig);

//    globalne varijable:
unsigned long num = 1;
char processing[] = "obrada.txt";
char status[] = "status.txt";

int end = 0;


int main() {

    struct sigaction act;
    /* 1. maskiranje signala SIGUSR1 */
    act.sa_handler = process_event; /* kojom se funkcijom signal obrađuje */
    sigemptyset(&act.sa_mask);
    sigaddset(&act.sa_mask, SIGTERM); /* blokirati i SIGTERM za vrijeme obrade */
    act.sa_flags = 0; /* naprednije mogućnosti preskočene */
    sigaction(SIGUSR1, &act, NULL); /* maskiranje signala preko sučelja OS-a */

    /* 2. maskiranje signala SIGTERM */
    act.sa_handler = process_sigterm;
    sigemptyset(&act.sa_mask);
    sigaction(SIGTERM, &act, NULL);

    /* 3. maskiranje signala SIGINT */
    act.sa_handler = process_sigint;
    sigaction(SIGINT, &act, NULL);
    printf("Program s PID=%ld krenuo s radom\n", (long) getpid());


    FILE *processingFile = fopen(processing, "a+"); //  1, 4, 9, 16, 25, ...
    FILE *statusFile = fopen(status, "a+"); // reading and writing, 1 -> 2 -> ...
    // wrote 1 in both file in first line with new line
    fscanf(statusFile, "%lu", &num);
    if (num == 0) {
        while (fscanf(processingFile, "%lu", &num) != EOF) {} //  if end of file (or an input error) occurs before any values are stored
        // broj = zadnji pročitani broj
        num = sqrt(num); // gcc main.c -o out -lm
    }
//    else {
//        freopen(processing, "w+", processingFile);
//        num = 0;
//    }

    // zapiši 0 u status.txt na početak datoteke (prepiši ono što je bilo!)
    freopen(status, "w+", statusFile); // deleting data in status.txt == "pepis"
    fprintf(statusFile, "%d", 0);
    fclose(statusFile);

    while (1) {
        num++;
        printf(" in main: %lu\n", num);
        unsigned long x = num * num;
        fprintf(processingFile, "%lu\n", x); // dodaj x u obrada.txt
        sleep(5); // cim nap SIGUSR1 prekid odma se prebaci na novi num ???, neceka 5 sec
        if(end) {
            fclose(processingFile);
            return 0;
        }
    }
}


void process_event(int sig) { // SIGUSR1        kill -SIGUSR1 14425
    printf("Primio signal SIGUSR1\n");
    printf("%lu\n", num);
}


void process_sigterm(int sig) { // SIGTERM      kill -SIGTERM 14299
    printf("Primio signal SIGTERM\n");
    FILE *statusFile = fopen(status, "w+");
    fprintf(statusFile, "%ld", num);
    fclose(statusFile);
    end = 1;
}


void process_sigint(int sig) { // SIGINT        kill -SIGINT 14296, Ctrl + C
    printf("Primio signal SIGINT, prekidam rad\n");
    end = 1;
}
