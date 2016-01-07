/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Laboratório: 9 - Atividade 1 */
/* Julia Anne de Souza Alves */

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define M 3 // Leitoras 
#define N 3 // Escritoras

pthread_mutex_t mutex;
pthread_cond_t cond_lei;
pthread_cond_t cond_esc;

int escritoras = 0, leitoras = 0, filaEsc = 0;

typedef struct aux
{
	int contador;
	int idThread;
} File;

File a;

void init_leitora(){
	pthread_mutex_lock(&mutex);

	while(escritoras > 0 || filaEsc > 0){
		pthread_cond_wait(&cond_lei, &mutex);
	}
	leitoras++;

	pthread_mutex_unlock(&mutex);
}

void exit_leitora(){
	pthread_mutex_lock(&mutex);

	leitoras--;
	if(leitoras == 0 || filaEsc > 0)
		pthread_cond_signal(&cond_esc);

	pthread_mutex_unlock(&mutex);
}

void init_escritora(){
	pthread_mutex_lock(&mutex);

	filaEsc++;
	while(escritoras>0){
		pthread_cond_wait(&cond_esc, &mutex);
	}
	escritoras++;

	pthread_mutex_unlock(&mutex);
}

void exit_escritora(){
	pthread_mutex_lock(&mutex);

	escritoras--;
	filaEsc--;

	pthread_cond_signal(&cond_esc);
	if(filaEsc == 0){
		pthread_cond_broadcast(&cond_lei);
	}

	pthread_mutex_unlock(&mutex);
}

void *leitora(void * arg){
	int tid = * (int *) arg;
	File aLocal;
	
	while(1){
		printf("Eu sou a thread leitora: %d \n", tid);
		init_leitora();
		aLocal.contador = a.contador;
		aLocal.idThread = a.idThread;
		printf("Contador: %d \n Gravado pela thread leitora %d \n", aLocal.contador, aLocal.idThread);
		exit_leitora();
		sleep(1);
	}

	pthread_exit(NULL);
	free(arg);
}

void *escritora(void *arg){
	int tid =  * (int *) arg;

	while(1){
		printf("Eu sou a thread escritora: %d \n", tid);
		init_escritora();
		pthread_mutex_lock(&mutex);
		a.contador++;
		a.idThread = tid;
		pthread_mutex_unlock(&mutex);

		exit_escritora();
		sleep(1);
	}

	pthread_exit(NULL);
	free(arg);
}

int main(int argc, char *argv[]){
	int *pid, i;
	pthread_t threads[M+N];
	
	for(i=0; i<N; i++){
		pid = (int*) malloc(sizeof(int));
		*pid = i;
		pthread_create(&threads[i], NULL, escritora, (void *) pid);
	}

	for(i=N; i<M+N; i++){
    		pid = (int*) malloc(sizeof(int));
	    	*pid = i;
    		pthread_create(&threads[i], NULL, leitora, (void *) pid);
	}

	for(i=0; i<M+N; i++){
        	pthread_join(threads[i],NULL);
	}

	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&cond_lei);
	pthread_cond_destroy(&cond_esc);
	
	free(pid);

	return 0;
}
