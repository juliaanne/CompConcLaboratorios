#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>
#include <semaphore.h>

#define LEITORES 4
#define ESCRITORES 4

sem_t em_l, em_e;

int leitores=0;

// ---------- ATENÇÃO
// Nessa solução, estarei dando prioridade aos leitores
// Isso acarreta que: Se não houver um último leitor, ou seja, sempre ouver um novo leitor concorrentemente,
// os escritores podem morrer de gome


// Em ESCRITORES, preciso apenas de um semáforo de exclusão mútua entre eles.

void InicEscritor(int pid){
	sem_wait(&em_e);
	// Exclusão mútua
	// Quando um escritor for escrever, apenas pegará o sinal ou esperará caso ele não exista.
}

void FimEscritor(int pid){
	sem_post(&em_e);
	// Exclusão mútua
	// Quando um escritor terminar, apenas deixará o sinal.
}

// Em LEITORES preciso completar a lógica

void InicLeitor(int pid){
	sem_wait(&em_l);

	leitores++;
	if(leitores == 1){
		// Se eu for o primeiro leitor, bloqueio o acesso pra escritores
		// Assim, enquanto houver leitores lendo, escritores não poderão escrever
		// Mas outros leitores poderão ler o arquivo
		sem_wait(&em_e);
	}		

	sem_post(&em_l);
}


void FimLeitor(int pid){
	sem_wait(&em_l);

	leitores--;
	if(leitores == 0){
		// Se eu for o ultimo leitor, libero o acesso para escritores
		// Ou seja, todos os leitores terminaram de ler, então poderá ser escrito
		sem_post(&em_e);
	}

	sem_post(&em_l);
}

void *Leitor(void *arg){
	int boba1, boba2, pid = * (int *) arg;

	while(1){
		printf("Eu sou a thread %d leitora e quero ler\n", pid);
		InicLeitor(pid);

		printf("Eu sou a thread %d leitora e estou lendo\n", pid);
		// Código para gastar tempo
		boba1=1000; boba2=-1000; while (boba2 < boba1) boba2++;

		FimLeitor(pid);
		printf("Eu sou a thread %d leitora e terminei de ler\n", pid);

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
		// Código para gastar tempo
		boba1=1000; boba2=-1000; while (boba2 < boba1) boba2++;

		FimEscritor(pid);
		printf("Eu sou a thread %d escritora e parei de escrever\n", pid);


	}

	free(arg);

	pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
	int nthreads = ESCRITORES + LEITORES;
	pthread_t threads[nthreads];
	int i, *pid;

	// Inicializando os semáforos
	sem_init(&em_l, 0, 1);
	sem_init(&em_e, 0, 1);

	// Criando as threads LEITORAS
	for (i = 0; i < LEITORES; i++){
		pid = malloc(sizeof(int));

		if(pid == NULL){
			printf("--ERRO: malloc() em alocação threads\n"); exit(-1);
		}
		*pid = i;

		pthread_create(&threads[i], NULL, Leitor, (void *) pid);
	}

	// Criando as threads ESCRITORAS
	for (i = LEITORES; i < nthreads; i++){
		pid = malloc(sizeof(int));

		if(pid == NULL){
			printf("--ERRO: malloc() em alocação threads\n"); exit(-1);
		}
		*pid = i;

		pthread_create(&threads[i], NULL, Escritor, (void *) pid);
	}

	// Esperando todas terminarem - só que não, rs
	for (i = 0; i < nthreads; i++) {
		pthread_join(threads[i], NULL);
	}

	return 0;
}