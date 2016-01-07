#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>
#include <semaphore.h>

#define NTHREADS 4

sem_t em_l, em_e;
sem_t lei, esc;

int escritores, leitores;

void InicLeitor(int pid){
	sem_wait(&em_l);

	while(escritores > 0){
		sem_wait(&lei);
	}		
	leitores++;

	sem_post(&em_l);
}


void FimLeitor(int pid){
	sem_wait(&em_l);

	leitores--;
	if(leitores == 0){
		sem_post(&lei);
	}

	sem_post(&em_l);
}

void InicEscritor(int pid){
	sem_wait(&em_e);

	while((leitores > 0) || (escritores > 0)) {
		sem_wait(&esc);
	}
	escritores++;

	sem_post(&em_e);
}

void FimEscritor(int pid){
	sem_wait(&em_e);

	escritores--;
	sem_post(&esc);
	sem_post(&lei);

	sem_post(&em_e);
}

void *Leitor(void *arg){
	int boba1, boba2, pid = * (int *) arg;

	while(1){
		printf("Eu sou a thread %d leitora e quero ler\n", pid);
		InicLeitor(pid);

		printf("Eu sou a thread %d leitora e estou lendo\n", pid);

		FimLeitor(pid);
		printf("Eu sou a thread %d leitora e terminei de ler\n", pid);

		boba1=1000; boba2=-1000; while (boba2 < boba1) boba2++;
	}

	free(arg);

	pthread_exit(NULL);
}

void *Escritor(void *arg){
	int boba1, boba2, pid = * (int *) arg;

	while(1){
		printf("Eu sou a thread %d escritora e quero escrever\n", pid);
		InicEscritor(pid);

		printf("Eu sou a thread %d escritora e estou escrevendo\n", pid);

		FimEscritor(pid);
		printf("Eu sou a thread %d escritora e parei de escrever\n", pid);

		boba1=1000; boba2=-1000; while (boba2 < boba1) boba2++;

	}

	free(arg);

	pthread_exit(NULL);
}

// Só preciso de um semaforo em escritores, de em.
// E completar a logica em leitores
// exclusão mutua para acessar variavel de controle de leitores
// se eu for a primeira, sem wait
// se eu for a ultima, sem post

int main(int argc, char *argv[]) {
	pthread_t threads[NTHREADS];
	int i, *pid;

	sem_init(&em_l, 0, 1);
	sem_init(&em_e, 0, 1);
	sem_init(&lei, 0, 0);
	sem_init(&esc, 0, 0);

	for (i = 0; i < NTHREADS/2; i++){
		pid = malloc(sizeof(int));

		if(pid == NULL){
			printf("--ERRO: malloc() em alocação threads\n"); exit(-1);
		}
		*pid = i;

		pthread_create(&threads[i], NULL, Leitor, (void *) pid);
	}

	for (i = NTHREADS/2; i < NTHREADS; i++){
		pid = malloc(sizeof(int));

		if(pid == NULL){
			printf("--ERRO: malloc() em alocação threads\n"); exit(-1);
		}
		*pid = i;

		pthread_create(&threads[i], NULL, Escritor, (void *) pid);
	}

	for (i = 0; i < NTHREADS; i++) {
		pthread_join(threads[i], NULL);
	}

	return 0;
}