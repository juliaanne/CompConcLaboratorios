/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Laboratório: 8 */
/* Julia Anne de Souza Alves */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NTHREADS 2
#define N 5

pthread_mutex_t mutex;
pthread_cond_t cond_cons;
pthread_cond_t cond_prod;
int count, Buffer[N], in, out;

int fibonacci(int n){
	int result;
	
	if(n==0 || n==1)
		return 1;

	result = fibonacci(n-1) + fibonacci(n-2);
	
	return result;
}

void Insere (int item) {
	pthread_mutex_lock(&mutex);

	while(count == N) {
		printf("PRODUTORA: vai se bloquear\n");
		pthread_cond_wait(&cond_prod, &mutex);
	}
	printf("PRODUTORA: vai inserir no buffer\n");
	Buffer[in] = item;
	int i; for(i=0; i<5; i++) printf("BUFFER: %d \n", Buffer[i]);
	in = (in + 1) % N;
	count++;

	printf("PRODUTORA: sinalizou uma consumidora\n");
	pthread_cond_signal(&cond_cons);
	pthread_mutex_unlock(&mutex);
}

int Retira (void) {
	int item;

	pthread_mutex_lock(&mutex);

	while(count == 0) {
		printf("CONSUMIDORA: vai se bloquear\n");
		pthread_cond_wait(&cond_cons, &mutex);
	}

	item = Buffer[out];
	printf("CONSUMIDORA: vai remover do buffer\n");
	Buffer[out] = 0;
	int i; for(i=0; i<5; i++) printf("BUFFER: %d \n", Buffer[i]);
	out = (out + 1) % N;
	count--;
	
	printf("CONSUMIDORA: vai sinalizar uma produtora\n");
	pthread_cond_signal(&cond_prod);
	pthread_mutex_unlock(&mutex);
	
	return item;
}

void *Produtora(void *t){
//	printf("PRODUTORA!\n");
	int item, i;
	
	for(i=0; i<25; i++){
		item = fibonacci(i);
		Insere(item);	
	}
	
	pthread_exit(NULL);
}

void *Consumidora(void *t){
//	printf("CONSUMIDORA!\n");
	Retira();
	
	pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
	int i; 
	pthread_t threads[NTHREADS];

	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init(&cond_cons, NULL);
	pthread_cond_init(&cond_prod, NULL);

	for(i=0; i<NTHREADS/2; i++){
		pthread_create(&threads[i], NULL, Produtora, NULL);
	}
	
	for(i=NTHREADS/2; i<NTHREADS; i++){
		pthread_create(&threads[i], NULL, Consumidora, NULL);
	}

	for (i = 0; i < NTHREADS; i++) {
		pthread_join(threads[i], NULL);
	}
	printf ("*\nFIM\n");
	
	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&cond_cons);
	pthread_cond_destroy(&cond_prod);
	
	return 0;
}
