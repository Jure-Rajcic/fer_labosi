#include <unistd.h>
#include <wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/shm.h>

int Id;     // segment id
int *process_var;
int thread_var;

void delete_memory(int sig) {
    (void) shmdt((char *) process_var);
    (void) shmctl(Id, IPC_RMID, NULL);
    exit(0);
}

void enter_thread_function() {
    while (*process_var != 0) {}
    *process_var = 0;
    int num = rand() % 100 + 1;
    printf("ULAZNI PROCES: broj %d\n", num);
    *process_var = num;
    for (int j = 0; j < 5; j++) sleep(1);
}

void *exit_thread_function(void *dummy_variable) {
    FILE *file = fopen("out.txt", "w+");
    for (int i = 0; i < *((int *) dummy_variable); ++i) {
        while (thread_var == 0) {}
        fprintf(file, "%d\n", thread_var);
        printf("IZLAZNA DRETVA: broj upisan u datoteku %d\n\n", thread_var);
        thread_var = 0;
    }
    fclose(file);
}


int main(int argc, char *argv[]) { 
    int num = atoi(argv[1]);
    printf("Pokrenuta RADNA DRETVA\n");
    // maskiranje signala SIGINT -> Ctrl C

    /* zauzimanje zajedničke memorije */
    Id = shmget(IPC_PRIVATE, sizeof(int), 0600);
    if (Id == -1) exit(1);  /* greška - nema zajedničke memorije */
    process_var = (int *) shmat(Id, NULL, 0);
    struct sigaction act;
    act.sa_handler = delete_memory; // u slučaju prekida briši memoriju
    sigaction(SIGINT, &act, NULL);

    *process_var = 0;
    thread_var = 0;

    if (fork() == 0) {
        // child
        printf("Pokrenut ULAZNI PROCES\n");
        srand(time(NULL));
        for (int i = 0; i < num; i++) {
            enter_thread_function();
        }
        exit(0);
    }
    //  parent == working_thread
    pthread_t exit_thread;
    if (pthread_create(&exit_thread, NULL, exit_thread_function, &num) != 0) {
        printf("GRESKA PRI STVARANJU IZLAZNE DRETVE!\n");
        exit(1);
    }
    printf("Pokrenuta IZLAZNA DRETVA\n\n");
    for (int i = 0; i < num; i++) {
        while (*process_var == 0) {}     // radno cekanje
        printf("RADNA DRETVA: pročitan broj %d i povećan na %d\n", *process_var, *process_var + 1);
        thread_var = *process_var + 1;
        while (thread_var != 0) {}
        *process_var = 0;
    }
    printf("Završila RADNA DRETVA\n");
    pthread_join(exit_thread, NULL);
    printf("Završila IZLAZNA DRETVA\n");
    wait(NULL);
    printf("Završio ULAZNI PROCES\n");

    delete_memory(0);
    return 0;
}

