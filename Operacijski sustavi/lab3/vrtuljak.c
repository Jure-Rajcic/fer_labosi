#include <stdio.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <wait.h>

#define NUM_OF_SEMAPHORES 4
sem_t *sem[NUM_OF_SEMAPHORES];
// -------------------------------------------------//
#define NUMBER_OF_SEATS 6
int NUMBER_OF_PASSENGERS = 13;
// -------------------------------------------------//

void visitor_process(int id) {
    sem_wait(sem[0]);
    printf("[%d.] Putnik: sjedoh\n", id);
    sem_post(sem[1]);
    sem_wait(sem[2]);
    printf("[%d.] Putnik: ustah\n", id);
    sem_post(sem[3]);
    exit(0);
}


void carousel_process(){
    while(NUMBER_OF_PASSENGERS >= NUMBER_OF_SEATS){
        for(int i = 0; i < NUMBER_OF_SEATS; i++){
            sem_post(sem[0]);
            sleep(1);
            sem_wait(sem[1]);
        }
        printf("Vrtuljak: Molimo putnike da se stave svoje pojaseve, voznja krece\n");
        sleep(3);
        printf("Vrtuljak: Dragi putnici voznja je zavrsila molimo vas da se iskrcate\n");
        for(int i = 0; i < NUMBER_OF_SEATS; i++){
            sem_post(sem[2]);
            sleep(1);
            sem_wait(sem[3]);
        }
        NUMBER_OF_PASSENGERS = NUMBER_OF_PASSENGERS - NUMBER_OF_SEATS;
        if(NUMBER_OF_PASSENGERS >= NUMBER_OF_SEATS){
            printf("Vrtuljak: Molimo putnike koji su cekali da se ukrcaju\n");
            for(int i = 0; i < NUMBER_OF_SEATS; i++){
                wait(NULL);
            }
        } else if(NUMBER_OF_PASSENGERS != 0){
            printf("Nažalost nema dovoljno putnika za sljedecu voznju, %d od %d", NUMBER_OF_PASSENGERS, NUMBER_OF_SEATS);
        }
    }
}

int main(){
    for(int i = 0; i < NUM_OF_SEMAPHORES; i++){
        int ID = shmget(IPC_PRIVATE,sizeof (sem_t), 0600);
        sem[i] = shmat(ID, NULL, 0);
        shmctl(ID,IPC_RMID, NULL);
    }
    sem_init(sem[0], 1, 0); // broj slobodnih mjesta ali polagani start
    sem_init(sem[1], 1, 0); // vrtuljak ceka popunjavanje svih mjesta
    sem_init(sem[2], 1, 0); // putnik ceka kraj voznje
    sem_init(sem[3], 1, 0); // putnika se diga

    pid_t pid;
    for(int i = 0; i < NUMBER_OF_PASSENGERS; i++){
        pid = fork();
        if(pid == -1){
            perror("Greska pri stvaranju procesa \n");
            exit(1);
        } else if(pid == 0){
            visitor_process(i + 1);
        }
    }
    carousel_process();
    for(int i = 0; i < NUM_OF_SEMAPHORES; i++){
        sem_destroy(sem[i]);
    }
    shmdt(*sem);
    return 0;
}

//gcc 1.c -pthread
//time ./a.out
