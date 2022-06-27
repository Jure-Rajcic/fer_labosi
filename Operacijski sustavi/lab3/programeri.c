#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include "pthread.h"

// -------------------------------------------------//
# define RESTURANT_MAX_CAPACITY 5
# define TOTAL_PROGRAMERS 14
// -------------------------------------------------//
# define MAX_MEAL_TIME 3

pthread_mutex_t m;
pthread_cond_t red[2];
char *strings[] = {"LINUX", "WINDOWS"};
int curr_res_capacity = 0, i = 0, last_id = 0, num_of_threads = 0;

void *programmer(void *p){
    int index = rand() & 1;
    pthread_mutex_lock(&m);
    num_of_threads++;
    last_id++;
    while(curr_res_capacity == RESTURANT_MAX_CAPACITY || (curr_res_capacity > 0 && i != index)){
        pthread_cond_wait(&red[index], &m);
    }
    curr_res_capacity++;
    i = index;
    printf("%s ENTERS RESTORAN (trenutno: %d/%d %s)\n",
           strings[index], curr_res_capacity, RESTURANT_MAX_CAPACITY, strings[index]); //udji
    pthread_mutex_unlock(&m);
    sleep(MAX_MEAL_TIME); // obavi
    pthread_mutex_lock(&m);
    curr_res_capacity--;
    if(curr_res_capacity > 0){
        pthread_cond_signal(&red[index]);
    } else {
        pthread_cond_broadcast(&red[1 - index]);
    }
    printf("%s EXIT RESTORAN(trenutno: %d/%d %s)\n%s",
           strings[index], curr_res_capacity, RESTURANT_MAX_CAPACITY, strings[index], curr_res_capacity == 0?"\n":""); // izadi
    num_of_threads--;
    pthread_mutex_unlock(&m);
    return NULL;
}

int main(){
    pthread_mutex_init(&m, NULL);
    pthread_cond_init(&red[0], NULL);
    pthread_cond_init(&red[1], NULL);

    pthread_t thr_id;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    // ovo samo da os obris esve resurse kad dretva zavrsi, netriba thread join

    srand((unsigned int) time(NULL));
    while (last_id < TOTAL_PROGRAMERS){
        pthread_create(&thr_id, &attr, programmer, NULL);
        sleep(2);
    }
    while (num_of_threads > 0){
        sleep(1);
    }

    pthread_mutex_destroy(&m);
    pthread_cond_destroy(&red[0]);
    pthread_cond_destroy(&red[1]);
}

//gcc programeri.c -pthread
//time ./a.out
