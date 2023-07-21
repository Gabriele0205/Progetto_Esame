//librerie importate
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <unistd.h>


sem_t sem_agente;
sem_t sem_tabacco;
sem_t sem_carta;
sem_t sem_fiammifero;
sem_t tabacco;
sem_t fiammifero;
sem_t carta;
sem_t sem_mutex;

bool tabaccoLibero = false;
bool cartaLibera = false;
bool fiammiferoLibero = false;

pthread_mutex_t print_mutex;

void *agenteA(void *);
void *agenteB(void *);
void *agenteC(void *);
void *pusherA(void *);
void *pusherB(void *);
void *pusherC(void *);
void *fumatore1(void *);
void *fumatore2(void *);
void *fumatore3(void *);

int main(int argc, char *argv[] ){
	
	pthread_t a1, a2, a3, p1, p2, p3, s1, s2, s3;
 	
	//inizializzazione dei semafori
    sem_init(&sem_agente, 0, 1);
    sem_init(&sem_tabacco, 0, 0);
    sem_init(&sem_carta, 0, 0);
    sem_init(&sem_fiammifero, 0, 0);
    sem_init(&tabacco, 0, 0);
    sem_init(&carta, 0, 0);
    sem_init(&fiammifero, 0, 0);  
    sem_init(&sem_mutex, 0, 1);
    pthread_mutex_init(&print_mutex, NULL);

    //creazione thread
    pthread_create(&a1, NULL, agenteA, NULL);
    pthread_create(&a2, NULL, agenteB, NULL);
    pthread_create(&a3, NULL, agenteC, NULL);
    pthread_create(&s1, NULL, fumatore1, NULL);
    pthread_create(&s2, NULL, fumatore2, NULL);
    pthread_create(&s3, NULL, fumatore3, NULL);
    pthread_create(&p1, NULL, pusherA, NULL);
    pthread_create(&p2, NULL, pusherB, NULL);
    pthread_create(&p3, NULL, pusherC, NULL);

    while(1){
    }
}

void *agenteA(void *a){
    while(1){
        sem_wait(&sem_agente);
        sem_post(&sem_tabacco);
        sem_post(&sem_carta);
    }
}

void *agenteB(void *b){
    while(1){
        sem_wait(&sem_agente);
        sem_post(&sem_tabacco);
        sem_post(&sem_fiammifero);
    }
}

void *agenteC(void *c){
    while(1){
        sem_wait(&sem_agente);
        sem_post(&sem_carta);
        sem_post(&sem_fiammifero);
    }
}