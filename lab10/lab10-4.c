/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Laboratório: 10 */
/* Julia Anne de Souza Alves */
/* Atividade 4 */

#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>
#include <semaphore.h>

#define NPROD 3
#define NCONS 3
#define N 5

int Buffer[N], IN=0, OUT=0;
sem_t slotVazio, slotCheio;
sem_t mutexProd, mutexCons;

// Função para imprimir o Buffer
// Imprime o Buffer apenas no Retirar, pois as threads Produtora e Consumidora acabariam imprimindo o buffer ao mesmo tempo causando confusão
void ImprimeBuffer(){
	int i;

	printf("Buffer: [");
	for(i=0; i<N; i++){
		printf("%d, ", Buffer[i]);
	}
	printf("]\n");
}

void Retira(){
	sem_wait(&slotCheio); // Aguarda slot cheio

	sem_wait(&mutexCons); // Exclusão mútua entre as Consumidoras

	Buffer[OUT] = -1;
	OUT = (OUT+1) % N;
	ImprimeBuffer();

	sem_post(&mutexCons);
	
	sem_post(&slotVazio); // Sinaliza slot vazio
}

void Insere(int elemento){
	sem_wait(&slotVazio); // Aguarda slot vazio
	
	sem_wait(&mutexProd); // Exclusão mútua entre as Produtoras
	
	Buffer[IN] = elemento;
	IN = (IN+1) % N;

	sem_post(&mutexProd);
	
	sem_post(&slotCheio); // Sinaliza slot cheio
}

void * Consumidora(void * arg){
	while(1){
		// Chama função de remover
		Retira(); 
	}

	pthread_exit(NULL);
}

void * Produtora(void * arg){
	int tid = * (int * ) arg, elemento;
	
	while(1){
		// Elemento igual a id da thread e chama função de inserir
		elemento = tid;
		Insere(elemento);
	}
	
	free(arg);
	pthread_exit(NULL);
}


int main(int argc, char *argv[]) {
	pthread_t tid[NCONS+NPROD];
	int t, *arg;

	// Inicializando semáforos
	sem_init(&mutexCons, 0, 1);
	sem_init(&mutexProd, 0, 1);
	sem_init(&slotCheio, 0, 0);
	sem_init(&slotVazio, 0, N);

	// Criando threads Produtoras, necessitam de id de threads
	for(t=0; t<NPROD; t++){
		arg = malloc(sizeof(int));
		if(arg == NULL){ printf("--ERRO: malloc()\n"); exit(-1); }
		*arg = t;
		if (pthread_create(&tid[t], NULL, Produtora, (void *) arg)) { printf("--ERRO: pthread_create()\n"); exit(-1); }
	}
	
	// Criando threads Consumidoras
	for(t=NPROD; t<NCONS+NPROD; t++){
		if (pthread_create(&tid[t], NULL, Consumidora, NULL)) { printf("--ERRO: pthread_create()\n"); exit(-1); }
	}

	//Aguardando todas as threads
	for (t=0; t<NCONS+NPROD; t++) {
		if (pthread_join(tid[t], NULL)) {
			printf("--ERRO: pthread_join() \n"); exit(-1); 
		}
	} 
	
	pthread_exit(NULL);

	sem_destroy(&mutexCons);
	sem_destroy(&mutexProd);
	sem_destroy(&slotVazio);
	sem_destroy(&slotCheio);

	return 0;
}
