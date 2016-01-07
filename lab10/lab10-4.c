/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Laboratório: 10 */
/* Julia Anne de Souza Alves */
/* Atividade 3 */

#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>
#include <semaphore.h>

#define NPROD 2
#define NCONS 2
#define N 5

int Buffer[N], IN=0, OUT=0;
sem_t slotVazio, slotCheio;
sem_t mutexProd, mutexCons;


void Retira(){
	sem_wait(&slotCheio);

	sem_wait(&mutexCons);

	Buffer[OUT] = -1;
	OUT = (OUT + 1) % N;

	sem_post(&mutexCons);
	
	sem_post(&slotVazio);
}

void Insere(int elemento){
	sem_wait(&slotVazio);
	
	sem_wait(&mutexProd);
	
	Buffer[IN] = elemento;
	IN = (IN + 1) % N;
	
	sem_post(&mutexProd);
	
	sem_post(&slotCheio);
}

void * Consumidora(void * arg){
	while(1){
		Retira();
	}

	pthread_exit(NULL);
}

void * Produtora(void * arg){
	int tid = * (int * ) arg, elemento;
	
	while(1){
		elemento = tid;
		Insere(elemento);
	}
	
	free(arg);
	pthread_exit(NULL);
}


int main(int argc, char *argv[]) {
	pthread_t tid[NCONS+NPROD];
	int t, *arg;

	sem_init(&mutexCons, 0, 1);
	sem_init(&mutexProd, 0, 1);
	sem_init(&slotCheio, 0, 0);
	sem_init(&slotVazio, 0, N);

	for(t=0; t<NPROD; t++){
		arg = malloc(sizeof(int));
	    if(arg == NULL){ printf("--ERRO: malloc()\n"); exit(-1); }
    	*arg = t;
		if (pthread_create(&tid[t], NULL, Produtora, (void *) arg)) { printf("--ERRO: pthread_create()\n"); exit(-1); }
	}
	
	for(t=NPROD; t<NCONS+NPROD; t++){
		if (pthread_create(&tid[t], NULL, Produtora, NULL)) { printf("--ERRO: pthread_create()\n"); exit(-1); }
	}


	for (t=0; t<NCONS+NPROD; t++) {
		if (pthread_join(tid[t], NULL)) {
			printf("--ERRO: pthread_join() \n"); exit(-1); 
		}
	} 
	
	pthread_exit(NULL);
	
	return 0;
}
