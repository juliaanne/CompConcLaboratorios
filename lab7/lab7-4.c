/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Laboratório: 7 - Atividade 4 */
/* Julia Anne de Souza Alves */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NTHREADS 4

/* Variaveis globais */
int controladora = 0;
pthread_mutex_t x_mutex;
pthread_cond_t x_cond;

/* Thread 'Tudo bem' */
void *A (void *t) {
	printf("1: Comecei\n");
		
	printf("Tudo bem?\n");

	pthread_mutex_lock(&x_mutex);
	controladora++;
	printf("1: controladora = %d, vai sinalizar a condicao \n", controladora);
	pthread_cond_broadcast(&x_cond);
	pthread_mutex_unlock(&x_mutex);

	pthread_exit(NULL);
}

/* Thread 'Hola' */
void *B (void *t) {
	printf("2: Comecei\n");
		
	printf("Hola!\n");

	pthread_mutex_lock(&x_mutex);
	controladora++;
	printf("2: controladora = %d, vai sinalizar a condicao \n", controladora);
	pthread_cond_broadcast(&x_cond);
	pthread_mutex_unlock(&x_mutex);

	pthread_exit(NULL);
}

/* Thread 'Até mais tarde' */
void *C (void *t) {
	printf("3: Comecei\n");

	pthread_mutex_lock(&x_mutex);
	if (controladora < 2) { 
		 printf("3: vai se bloquear...\n");
		 pthread_cond_wait(&x_cond, &x_mutex);
		 printf("3: sinal recebido e mutex realocado, controladora = %d\n", controladora);
	}
	pthread_mutex_unlock(&x_mutex); 
	printf("Até mais tarde\n");
	pthread_exit(NULL);
}

/* Thread 'Tchau' */
void *D (void *t) {
	printf("4: Comecei\n");

	pthread_mutex_lock(&x_mutex);
	if (controladora < 2) { 
		 printf("4: vai se bloquear...\n");
		 pthread_cond_wait(&x_cond, &x_mutex);
		 printf("4: sinal recebido e mutex realocado, controladora = %d\n", controladora);
	}
	pthread_mutex_unlock(&x_mutex); 
	printf("Tchau\n");
	pthread_exit(NULL);
}

/* Funcao principal */
int main(int argc, char *argv[]) {
	int i; 
	pthread_t threads[NTHREADS];

	/* Inicilaiza o mutex (lock de exclusao mutua) e a variavel de condicao */
	pthread_mutex_init(&x_mutex, NULL);
	pthread_cond_init (&x_cond, NULL);

	/* Cria as threads */
	pthread_create(&threads[0], NULL, A, NULL);
	pthread_create(&threads[1], NULL, B, NULL);
	pthread_create(&threads[2], NULL, C, NULL);
	pthread_create(&threads[3], NULL, D, NULL);

	/* Espera todas as threads completarem */
	for (i = 0; i < NTHREADS; i++) {
		pthread_join(threads[i], NULL);
	}
	printf ("*\nFIM\n");
	
	/* Desaloca variaveis e termina */
	pthread_mutex_destroy(&x_mutex);
	pthread_cond_destroy(&x_cond);
	pthread_exit (NULL);
}
