/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Laboratório: 3 */
/* Julia Anne de Souza Alves */

#include <stdio.h>
#include <stdlib.h> 
#include "timer.h"  //para usar funcoes de medida de tempo
#include <unistd.h> //para usar a funcao "sysconf"
#include <string.h>
#include <pthread.h>

double **mat, **matresult; 

typedef struct {
   int idThread, nTam, nThreads;
} t_Args;

void *multiplica(void *arg){

	int i, c, result;
	t_Args *args = (t_Args *) arg;
	
	int l = args->idThread;
	int nTam = args->nTam;

	for( ; l<nTam; l+=args->nThreads){
		for(c=0; c<nTam; c++){
			result=0;
			for(i=0; i<nTam; i++) 
				result += mat[l][i] * mat[i][c];
			matresult[l][c]=result;
		}
	}

	free(arg);
	pthread_exit(NULL);
}


int main(int argc, char *argv[]){ 

	int t, l, c, nlinhas, ncolunas, nthreads;
	double inicio, fim, delta1, delta2, delta3;

	FILE *arquivo;
	char nomeArquivo[1000];
	
	t_Args *arg; //receberá os argumentos para a thread

	//---------------------------------------------------------------- Parte 1: Inicialização
	
	GET_TIME(inicio);
	
	//Validando e recebendo a entrada
	if(argc < 3) {
		printf("Use: %s <arquivo entrada> <numero threads>\n", argv[0]);
		exit(EXIT_FAILURE);
  	}
	
	//Pegando o nome do arquivo	
	strcpy(nomeArquivo, argv[1]);
	
	//Pegando numero de threads
	nthreads = atoi(argv[2]);

	pthread_t tid_sistema[nthreads];

	//Lendo arquivo
	printf("Nome arquivo: %s \n", nomeArquivo);
	arquivo = fopen(nomeArquivo, "r");

	//Verificando problemas na abertura do arquivo
	if(arquivo == NULL){
		printf("-- Falha na abertura do arquivo \n");
		exit(EXIT_FAILURE);
	}

	//Definindo tamanho da matriz
	fscanf(arquivo, "%d %d", &nlinhas, &ncolunas);
	//printf("Numero de linhas: %d\nNumero de colunas: %d \n", nlinhas, ncolunas);

	//Alocando espaço para matriz de entrada
	mat = (double**) malloc(sizeof(double) * nlinhas);

	for(l=0; l<ncolunas; l++){
        	mat[l] = (double *) malloc(sizeof(double) * ncolunas);
	}

	//Alocando espaço para matriz de saída
	matresult = (double**) malloc(sizeof(double) * nlinhas);

	for(l=0; l<ncolunas; l++){
        	matresult[l] = (double *) malloc(sizeof(double) * ncolunas);
	}

	if(mat==NULL || matresult==NULL) {
		printf("--ERRO: malloc()\n"); 
		exit(-1);
	}

	//Inicializando a matriz de entrada
	for(l=0; l<nlinhas; l++){
		//printf("\n------------ Linha: %d ------------\n", l);
		for(c=0; c<ncolunas; c++){
			fscanf(arquivo, "%lf", &mat[l][c]);
			//printf("%.2lf", mat[l][c]);
		}
	}

	fclose(arquivo);
	
	GET_TIME(fim);

  	delta1 = fim - inicio;
  	

 	//---------------------------------------------------------------- Parte 2: Execução

	GET_TIME(inicio);

	for(t=0; t<nthreads; t++) {
		printf("--Aloca e preenche argumentos para thread %d\n", t);
		arg = malloc(sizeof(t_Args));
	    if (arg == NULL) {
			printf("--ERRO: malloc()\n"); exit(-1);
    	}
    	arg->idThread = t; 
    	arg->nTam=nlinhas;
    	arg->nThreads=nthreads;
    
    	printf("--Cria a thread %d\n", t+1);
    	if (pthread_create(&tid_sistema[t], NULL, multiplica, (void*) arg)) {
			printf("--ERRO: pthread_create()\n"); exit(-1);
		}
	}
	
	//Espera as threads terminarem
	for (t=0; t<nthreads; t++) {
		if (pthread_join(tid_sistema[t], NULL)) {
			printf("--ERRO: pthread_join() \n"); exit(-1); 
		} 
	}
	
	GET_TIME(fim);

  	delta2 = fim - inicio;


 	//---------------------------------------------------------------- Parte 3: Finalização

	GET_TIME(inicio);
	


	/*for(l=0; l<nlinhas; l++){
		for(c=0; c<ncolunas; c++){
			printf("%.2lf ", matresult[l][c]);
		}
		printf("\n");
	}*/
	printf("Terminamos a multiplicação");

	free(mat);
	free(matresult);

	GET_TIME(fim);

  	delta3 = fim - inicio;


	//Exibição dos tempos
	printf("Tempo inicializacao: %.8lf\n", delta1);
	printf("Tempo execucaos: %.8lf\n", delta2);
	printf("Tempo finalizacao: %.8lf\n", delta3);
	return 0;
  
}
