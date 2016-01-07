/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Laboratório: 5 */
/* Julia Anne de Souza Alves */

#include <stdio.h>
#include <stdlib.h> 
//#include "timer.h"  //para usar funcoes de medida de tempo
#include <math.h> 
#include <string.h>
#include <pthread.h>

int nThreads, nElem;
float *vetAuxParalelo, *vetAuxSequencial;

void *calculaPiParalelo(void *arg)
{
	float piParcial = 0;
	int i, numerador;

	int tid = * (int *) arg;

	//Calculando os pis parciais
	for(i=tid; i<nElem; i+=nThreads)
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
	
	for(c=0; c<nThreads; c++){
		piParcial=0;
		for(i=c; i<nElem; i+=nThreads){
			numerador = (i%2==0)? 1.0 : -1.0;
			piParcial += (numerador/(2.0*i+1.0));
		}
		vetAuxSequencial[c]=4*piParcial;
	}
}


int main(int argc, char *argv[])
{ 
	//double inicio, fim, delta1, delta2, delta3;
	float piParalelo, piSequencial, piAssociativo;
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
	nThreads = atoi(argv[2]);

	pthread_t tid_sistema[nThreads];
	
	//Alocando espaço para o vetor auxiliar paralelo
	vetAuxParalelo = (float*) malloc(sizeof(float) * nThreads);
  	
	//Alocando espaço para o vetor auxiliar sequencial
	vetAuxSequencial = (float*) malloc(sizeof(float) * nThreads);
	
 	//---------------------------------------------------------------- Parte 2: Execução

	//Criando as threads e chamando a função que calcula o valor de pi paralelamente
	for(t=0; t<nThreads; t++)
	{
		printf("--Aloca e preenche argumentos para thread %d\n", t);
		
		arg = malloc(sizeof(int));
	    if (arg == NULL) 
	    {
			printf("--ERRO: malloc()\n"); exit(-1);
    	}
    	*arg = t;
    	
    	printf("--Cria a thread %d\n", t+1);
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
	
	// Calculando o valor de pi sequencial
	piSequencial = calculaPiSequencial();
	
	//Calculando o valor de pi paralelo
	for(i=0; i<nThreads; i++){
		piParalelo += vetAuxParalelo[i];
	}
	
	// Calculando o valor de pi sequencial associativo
	calculaPiAssociativo();
	
	//Calculando o valor de pi sequencial associativo
	for(i=0; i<nThreads; i++){
		//printf("Vetor Auxiliar Sequencial [%d] é: %.10f\n", i, vetAuxSequencial[i]); 
		piAssociativo += vetAuxSequencial[i];
	}

 	//---------------------------------------------------------------- Parte 3: Finalização

	printf("\n");
	printf("O valor de M_PI é: %.15f\n", M_PI);
	printf("O valor de pi sequencial é: %.15f\n", piSequencial);
	printf("O valor de pi paralelo é: %.15f\n", piParalelo);
	printf("O valor de pi sequencial associativo é: %.15f\n", piAssociativo);
	
	free(vetAuxParalelo);
	free(vetAuxSequencial);

	return 0; 
}
