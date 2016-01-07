/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Laboratório: 7 - Atividade 5 */
/* Julia Anne de Souza Alves */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NTHREADS  5
#define PASSOS  5

/* Variaveis globais */
int controladora = 0;
pthread_mutex_t x_mutex;
pthread_cond_t x_cond;

/* Funcao barreira */
void barreira(int nthreads)
{
	//Lock para fazer as alterações necessárias em controladora
	pthread_mutex_lock(&x_mutex);
	controladora++;
  	
	if(controladora < nthreads){ // Se todas as threads ainda não terminaram, espera
		pthread_cond_wait(&x_cond, &x_mutex);
	} else { // A ultima thread, a quinta a executar, sinaliza todas as outras 
		controladora = 0; 
		pthread_cond_broadcast(&x_cond);
	}
	
	// Libera o lock para que outra thread possa pegar.
	pthread_mutex_unlock(&x_mutex);	 
}

void * A (void * arg) {
	int tid = * (int *) arg, i;
	int cont = 0, boba1, boba2;
	
	for (i=0; i < PASSOS; i++) {
		cont++;
		printf("Thread %d: cont=%d, passo=%d\n", tid, cont, i);
		//sincronizacao condicional
		barreira(NTHREADS);
		/* faz alguma coisa inutil pra gastar tempo... */
		boba1=100; boba2=-100; while (boba2 < boba1) boba2++;
	}
	
	free(arg);
	pthread_exit(NULL);
}

/* Funcao principal */
int main(int argc, char *argv[]) {
	int i, * tid; 
	pthread_t threads[NTHREADS];

	/* Inicilaiza o mutex (lock de exclusao mutua) e a variavel de condicao */
	pthread_mutex_init(&x_mutex, NULL);
	pthread_cond_init (&x_cond, NULL);

	/* Cria as threads */
	for (i = 0; i < NTHREADS; i++) {
		tid = (int *) malloc(sizeof(int));
        	*tid = i;
		pthread_create(&threads[i], NULL, A,(void *) tid);	}

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
