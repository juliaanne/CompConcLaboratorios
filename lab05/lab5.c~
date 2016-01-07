/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Laboratório: 5 */
/* Julia Anne de Souza Alves */


/*
O calculo do pi paralelo de forma que o for salta o numero de threads, ou seja, a primeira thread calcula os valores da serie de x em x, sendo x o numero das threads, favorece um erro peculiar.

Como o exercicio é pedido com 2 threads, a primeira thread calcula sempre os valores positivos da série. E a segunda thread calcula sempre os valores negativos da thread. Então o primeiro valor é numericamente grande e o segundo é numericamente pequeno. Quando a soma dessas duas variáveis float acontece, há uma perda que causa uma diferença significativa no valor de pi - por conta do problema da associatividade do float.

Quando calculamos da mesma forma com 3 threads isso não acontece, pois não há o favorecimento do erro.

A solução implementada foi calcular os valores paralelos através de blocos. Ou seja, no caso de duas threads, a primeira thread faz o calculo da primeira metade da série e a segunda faz o calculo da segunda metade não favorecendo o erro na hora da soma desses dois valores.
*/

#include <stdio.h>
#include <stdlib.h> 
#include "timer.h"
#include <math.h> 
#include <string.h>
#include <pthread.h>

int nThreads;
long long int nElem;
float *vetAuxParalelo, *vetAuxSequencial;

void *calculaPiParalelo(void *arg)
{
	float piParcial = 0;
	int i, numerador;
	long long int ini, fim, bloco;

	int tid = * (int *) arg;

	//Calculando tamanho do bloco, posicao inicial e final de cada thread
	bloco = nElem/nThreads;
	ini = tid*bloco;
	fim = ini + bloco;

	//A ultima thread calcula os ultimos elementos, se houver
	if(tid==(nThreads-1))
		fim = nElem;

	//Calculando os pis parciais
	for(i=ini; i<fim; i++)
	{
		numerador = (i%2==0)? 1.0 : -1.0;
		piParcial += (numerador/(2.0*i+1.0));
	}

	//Alocando resultado no vetor de saída
	vetAuxParalelo[tid] = 4*piParcial;
	
	//Liberando espaços e retornando
	free(arg);
	pthread_exit(NULL);
}

float calculaPiSequencial()
{
	float piSequencial = 0;
	int i, numerador;
	
	for(i=0; i<nElem; i++)
	{
		numerador = (i%2==0)? 1.0 : -1.0;
		piSequencial += (numerador/(2.0*i+1.0));
	}
	
	return 4*piSequencial;
}

void calculaPiAssociativo()
{
	int i, c, numerador;
	float piParcial;
	long long int ini, fim, bloco;

	bloco = nElem/nThreads;

	for(c=0; c<nThreads; c++){
		piParcial=0;
		ini = c*bloco;
		fim = ini + bloco;
		for(i=ini; i<fim; i++){
			numerador = (i%2==0)? 1.0 : -1.0;
			piParcial += (numerador/(2.0*i+1.0));
		}
		vetAuxSequencial[c]=4*piParcial;
	}
}


int main(int argc, char *argv[])
{ 
	double inicio, fim, tempoParalelo, tempoAssociativo, tempoSequencial;
	float piParalelo=0, piSequencial=0, piAssociativo=0;
	int t, i, *arg;

	//---------------------------------------------------------------- Parte 1: Inicialização
	
	//Validando e recebendo a entrada
	if(argc < 3) {
		printf("Use: %s <numero elementos> <numero threads>\n", argv[0]);
		exit(EXIT_FAILURE);
  	}
	
	//Pegando numero de elementos da série
	nElem = atoi(argv[1]);
	
	//Pegando numero de threads
	nThreads = atoll(argv[2]);

	pthread_t tid_sistema[nThreads];
	
	//Alocando espaço para o vetor auxiliar paralelo
	vetAuxParalelo = (float*) malloc(sizeof(float) * nThreads);
  	
	//Alocando espaço para o vetor auxiliar sequencial
	vetAuxSequencial = (float*) malloc(sizeof(float) * nThreads);
	
 	//---------------------------------------------------------------- Parte 2: Execução

	GET_TIME(inicio);

	//Criando as threads e chamando a função que calcula o valor de pi paralelamente
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
    	if (pthread_create(&tid_sistema[t], NULL, calculaPiParalelo, (void*) arg))
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


	//Calculando o valor de pi paralelo
	for(i=0; i<nThreads; i++){
		piParalelo += vetAuxParalelo[i];
	}

	GET_TIME(fim);

	tempoParalelo = fim - inicio;

	
	GET_TIME(inicio);

	// Calculando o valor de pi sequencial
	piSequencial = calculaPiSequencial();

	GET_TIME(fim);

	tempoSequencial = fim - inicio;


	GET_TIME(inicio);
	
	// Calculando o valor de pi sequencial associativo
	calculaPiAssociativo();

	//Calculando o valor de pi sequencial associativo
	for(i=0; i<nThreads; i++){
		piAssociativo += vetAuxSequencial[i];
	}

	GET_TIME(fim);

	tempoAssociativo = fim - inicio;

 	//---------------------------------------------------------------- Parte 3: Finalização

	//Exibição dos resultados
	printf("\n");
	printf("O valor de M_PI é: %.15f\n", M_PI);
	printf("O valor de pi sequencial é: %.15f\n", piSequencial);
	printf("O valor de pi paralelo é: %.15f\n", piParalelo);
	printf("O valor de pi sequencial associativo é: %.15f\n", piAssociativo);
	
	//Exibição dos tempos
	printf("\n");
	printf("Tempo do calculo sequencial: %.8lf\n", tempoSequencial);
	printf("Tempo do calculo paralelo: %.8lf\n", tempoParalelo);
	printf("Tempo do calculo sequencial associativo: %.8lf\n", tempoAssociativo);
	free(vetAuxParalelo);
	free(vetAuxSequencial);

	return 0; 
}
