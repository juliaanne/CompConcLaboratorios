/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Laboratório: 10 */
/* Julia Anne de Souza Alves */
/* Atividade 3 */

#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>
#include <semaphore.h>

#define NTHREADS 4

sem_t semaforo, semaforoexc;
int controladora = 0;

void *A (void * arg){
	int i, tid = * (int *) arg;
	
	printf("%d: Comecei\n", tid);	
	printf("Olá. Tudo bem? \n");

	sem_wait(&semaforoexc);
	controladora++;
	if (controladora == 2) {
		for(i=0; i<NTHREADS/2; i++)
			sem_post(&semaforo);
	} else { printf("Não sinalizei. Ainda esperando."); }
	sem_post(&semaforoexc);
	
	printf("Thread: %d terminou!\n", tid);

	free(arg);
	pthread_exit(NULL);
}

void *B (void * arg){
	int i, tid = * (int *) arg;
	
	printf("%d: Comecei\n", tid);	
	printf("Hello! \n");

	sem_wait(&semaforoexc);
	controladora++;
	if (controladora == 2) { 
		for(i=0; i<NTHREADS/2; i++)
			sem_post(&semaforo); 
	} else { printf("Não sinalizei. Ainda esperando."); }
	sem_post(&semaforoexc);

	printf("Thread: %d terminou!\n", tid);
	
	free(arg);
	pthread_exit(NULL);
}


void *C (void * arg){
	int tid = * (int *) arg;
	
	printf("%d: Comecei\n", tid);	

	sem_wait(&semaforo);    

	printf("Até mais tarde! \n");
	
	printf("Thread: %d terminou!\n", tid);

	free(arg);
	pthread_exit(NULL);
}

void *D (void * arg){
	int tid = * (int *) arg;
	
	printf("%d: Comecei\n", tid);	

	sem_wait(&semaforo);    

	printf("Tchau! \n");
	
	printf("Thread: %d terminou!\n", tid);
	
	free(arg);
	pthread_exit(NULL);
}


int main(int argc, char *argv[]) {
	pthread_t tid[NTHREADS];
	int t, *id[4];

	for (t=0; t<NTHREADS; t++) {
		if ((id[t] = malloc(sizeof(int))) == NULL) {
		pthread_exit(NULL); return 1;
		}
		*id[t] = t+1;
	}

	sem_init(&semaforo, 0, 0);
	sem_init(&semaforoexc, 0, 1);

	if (pthread_create(&tid[1], NULL, C, (void *) id[2])) { printf("--ERRO: pthread_create()\n"); exit(-1); }
	if (pthread_create(&tid[0], NULL, D, (void *) id[3])) { printf("--ERRO: pthread_create()\n"); exit(-1); }

	if (pthread_create(&tid[3], NULL, A, (void *) id[0])) { printf("--ERRO: pthread_create()\n"); exit(-1); }
	if (pthread_create(&tid[2], NULL, B, (void *) id[1])) { printf("--ERRO: pthread_create()\n"); exit(-1); }


	for (t=0; t<NTHREADS; t++) {
		if (pthread_join(tid[t], NULL)) {
		printf("--ERRO: pthread_join() \n"); exit(-1); 
		}
		
		free(id[t]);
	} 
	
	pthread_exit(NULL);
	
	return 0;
}
