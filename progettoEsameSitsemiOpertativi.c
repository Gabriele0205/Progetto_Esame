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
 
 	
 	/* ****modifica dei valori con cui vengono inizializzati i semafori
 		   in modo da far andare in stallo il processo**** 
	sem_init(&sem_agente, 0, 0);
    sem_init(&sem_tabacco, 0, 1);
    sem_init(&sem_carta, 0, 1);
    sem_init(&sem_fiammifero, 0, 1);
    sem_init(&tabacco, 0, 1);
    sem_init(&carta, 0, 1);
    sem_init(&fiammifero, 0, 1);  
    sem_init(&sem_mutex, 0, 0);
    */

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

void *fumatore1(void *a){
	while(1){
		pthread_mutex_lock(&print_mutex);
		printf("S1 ha bisogno di tabacco\n");
		pthread_mutex_unlock(&print_mutex);
		
		sem_wait(&tabacco);

		pthread_mutex_lock(&print_mutex);
		printf("S1 ottiene il tabacco e si arrotola una sigaretta \n");
		pthread_mutex_unlock(&print_mutex);
		
		sem_post(&sem_agente);

		pthread_mutex_lock(&print_mutex);
		printf("S1 ne arrotola una e fa cadere il tabacco\n" );
		pthread_mutex_unlock(&print_mutex);
		sleep(4);
	}
}

void *fumatore2(void *b){
	while(1){
		pthread_mutex_lock(&print_mutex);
		printf("S2 ha bisogno di carta\n");
		pthread_mutex_unlock(&print_mutex);

		sem_wait(&carta);

		pthread_mutex_lock(&print_mutex);
		printf("S2 ottiene la carta,arrotola una sigaretta\n");
		pthread_mutex_unlock(&print_mutex);

		sem_post(&sem_agente);
		pthread_mutex_lock(&print_mutex);
		printf("S2 ne arroltola una e fa cadere le cartine\n" );
		pthread_mutex_unlock(&print_mutex);
		sleep(4);
	}
}

void *fumatore3(void *c){
	while(1){
		pthread_mutex_lock(&print_mutex);
		printf("S3 ha bisogno dei fiammiferi\n");
		pthread_mutex_unlock(&print_mutex);

		sem_wait(&fiammifero);
		pthread_mutex_lock(&print_mutex);
		printf("S3 ottiene i fiammiferi e arrotola una sigaretta \n");
		pthread_mutex_unlock(&print_mutex);
		sem_post(&sem_agente);

		pthread_mutex_lock(&print_mutex);
		printf("S3 ne arrotola una e fa cadere i fiammiferi \n" );
		pthread_mutex_unlock(&print_mutex);

		sleep(4);
	}
}

void *pusherA(void *a){
    while(1){
            sem_wait(&sem_tabacco);
            pthread_mutex_lock(&print_mutex);
            printf("Il tabacco è sul tavolo.\n");
            pthread_mutex_unlock(&print_mutex);

            sem_wait(&sem_mutex);
                if(cartaLibera){
                    cartaLibera = false;
                    sem_post(&carta);
                }else if(fiammiferoLibero){
                    fiammiferoLibero = false;
                    sem_post(&fiammifero);
                }else{
                    tabaccoLibero = true;
                }
            sem_post(&sem_mutex);
    }
}


void *pusherB(void *b){
    while(1){
        sem_wait(&sem_fiammifero);
        pthread_mutex_lock(&print_mutex);
        printf("I fiammiferi sono sul tavolo.\n");
        pthread_mutex_unlock(&print_mutex);

        sem_wait(&sem_mutex);
            if(cartaLibera){
                cartaLibera = false;
                sem_post(&fiammifero);
            }else if(tabaccoLibero){
                tabaccoLibero = false;
                sem_post(&tabacco);
            }else{
                fiammiferoLibero = true;
            }
        sem_post(&sem_mutex);
    }
}

void *pusherC(void *c){
    while(1){
        sem_wait(&sem_carta);
        pthread_mutex_lock(&print_mutex);
        printf("Le cartine sono sul tavolo.\n");
        pthread_mutex_unlock(&print_mutex);

        sem_wait(&sem_mutex);
            if(tabaccoLibero){
                tabaccoLibero = false;
                sem_post(&tabacco);
            }else if(fiammiferoLibero){
                fiammiferoLibero = false;
                sem_post(&fiammifero);
            }else{
                cartaLibera = true;
            }
        sem_post(&sem_mutex);
    }
}
