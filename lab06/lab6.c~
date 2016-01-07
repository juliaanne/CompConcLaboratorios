/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Laboratório: 6 */
/* Julia Anne de Souza Alves */

#include <stdio.h>
#include <stdlib.h> 
#include <math.h>
#include <string.h>
#include <pthread.h>
#include "timer.h"

int nThreads;
long long int n, *tot_threads, i_global=1;
pthread_mutex_t mutex;

int ehPrimo(long long int n)
{
	int i;
	
	if (n<=1) return 0;
	if (n==2) return 1;
	if (n%2==0) return 0;
	for (i=3; i<sqrt(n)+1; i+=2)
		if(n%i==0) return 0;
	return 1;

}

void *contaPrimos(void *arg)
{
	int tid = * (int *) arg;

	long long int total = 0, i_local;

	pthread_mutex_lock(&mutex);
	i_local = i_global;
	i_global++;
     pthread_mutex_unlock(&mutex);
     	
	//Calculando primos
	while(i_local <=n)
	{
		if(ehPrimo(i_local)) 
			total++;
		pthread_mutex_lock(&mutex);
		i_local = i_global;
		i_global++;
	     pthread_mutex_unlock(&mutex);
	}
	
	//Colocando no vetor resultado
	tot_threads[tid] = total;
	
	//Liberando espaços e retornando
	free(arg);
	pthread_exit(NULL);
}

int main(int argc, char *argv[])
{ 
	double ini, fim;
	int t ,*arg;
	long long int i, nPrimosSequencial=0, nPrimosParalelo=0;

	//---------------------------------------------------------------- Parte 1: Inicialização
	
	//Validando e recebendo a entrada
	if(argc < 3) {
		printf("Use: %s <numero elementos> <numero threads>\n", argv[0]);
		exit(EXIT_FAILURE);
  	}
	
	//Pegando numero de elementos da série
	n = atoll(argv[1]);
	
	//Pegando numero de threads
	nThreads = atoi(argv[2]);
	
	//Alocando espaço para o vetor auxiliar paralelo
	tot_threads = (long long int*) malloc(sizeof(long long int) * nThreads);

	pthread_t tid_sistema[nThreads];
	
 	//---------------------------------------------------------------- Parte 2: Execução
	pthread_mutex_init(&mutex, NULL);
	
	//Executando a função sequencial
	
	GET_TIME(ini);
	
	for(i=0; i<=n; i++)
	{
		nPrimosSequencial += ehPrimo(i);
	}
	
	GET_TIME(fim);
	
	printf("Tempo Sequencial: %lf\n", fim-ini);


	GET_TIME(ini);	 
	//Criando as threads e chamando a função paralela
	for(t=0; t<nThreads; t++)
	{
		//printf("--Aloca e preenche argumentos para thread %d\n", t);
		
		arg = malloc(sizeof(int));
	    if (arg == NULL) 
	    {
			printf("--ERRO: malloc()\n"); exit(-1);
    	}
    	*arg = t;
    	
    	//printf("--Cria a thread %d\n", t+1);
    	if (pthread_create(&tid_sistema[t], NULL, contaPrimos, (void*) arg))
    	 {
			printf("--ERRO: pthread_create()\n"); exit(-1);
		}
	}
	
	//Espera as threads terminarem
	for(t=0; t<nThreads; t++)
	{
		if (pthread_join(tid_sistema[t], NULL))
		{
			printf("--ERRO: pthread_join() \n"); exit(-1); 
		} 
	}
	
	pthread_mutex_destroy(&mutex);	
	
	//Calculando o valor primo paralelo
	for(i=0; i<nThreads; i++){
		nPrimosParalelo += tot_threads[i];
	}
	
	GET_TIME(fim);
	
	printf("Tempo Concorrente: %lf\n", fim-ini);

 	//---------------------------------------------------------------- Parte 3: Finalização

	printf("- Algoritmo Sequencial: \nO numero de primos em %lld é: %lld\n", n, nPrimosSequencial);
	printf("- Algoritmo Paralelo: \nO numero de primos em %lld é: %lld\n", n, nPrimosParalelo);
	
	return 0; 
}
