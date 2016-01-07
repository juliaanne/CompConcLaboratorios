/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Laboratório: 2 */
/* Julia Anne de Souza Alves */

#include <stdio.h>
#include <stdlib.h> 
#include "timer.h"  //para usar funcoes de medida de tempo
#include <unistd.h> //para usar a funcao "sysconf"
#include <string.h>

double **mat, **matresult; //Tentei com float, mas por alguma razão dá segmentation fault.

int multiplica(double **mat, int nlinhas, int ncolunas){

	int i, l, c, result;

	for(l=0; l<nlinhas; l++){
		for(c=0; c<ncolunas; c++){
			result=0;
			for(i=0; i<nlinhas; i++) 
				result += mat[l][i] * mat[i][c];
			matresult[l][c]=result;
		}
	}

	return 0;
}


int main(int argc, char *argv[]){ 

	int l, c, nlinhas, ncolunas; //result;
	double inicio, fim, delta1, delta2, delta3;

	FILE *arquivo;
	char nomeArquivo[1000];

	//---------------------------------------------------------------- Parte 1: Inicialização
	
	GET_TIME(inicio);
	
	//Validando e recebendo a entrada
	if(argc < 2) {
		printf("Use: %s <arquivo entrada> \n", argv[0]);
		exit(EXIT_FAILURE);
  	}
	
	//Pegando o nome do arquivo	
	strcpy(nomeArquivo, argv[1]);

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

	multiplica(mat, nlinhas, ncolunas);

	GET_TIME(fim);

  	delta2 = fim - inicio;


 	//---------------------------------------------------------------- Parte 3: Finalização

	GET_TIME(inicio);

	for(l=0; l<nlinhas; l++){
		for(c=0; c<ncolunas; c++){
			printf("%.2lf ", matresult[l][c]);
		}
		printf("\n");
	}

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
