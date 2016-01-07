/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Laboratório: 2 */
/* Julia Anne de Souza Alves */

#include <stdio.h>
#include <stdlib.h> 
#include "timer.h"
#include <unistd.h>


float *vet;
int tam;
	
	
int main(int argc, char *argv[]){ 

	int t; //result;
	double inicio, fim, delta1; //delta2, delta3;
	FILE *arquivo;
	char nomeArquivo[100];

	// Parte de Inicialização
	
	GET_TIME(inicio);
	
	//Validando e recebendo a entrada
	if(argc < 2) {
		printf("Use: %s <arquivo entrada>\n", argv[0]);
		exit(EXIT_FAILURE);
  	}
	

	
	arquivo =fopen("");

	//Alocando memoria e inicializando o vetor da entrada
	vet = malloc (sizeof(float) * tam);

	if(vet==NULL) {
		printf("--ERRO: malloc()\n"); exit(-1);
	}

	for(t=0; t<tam; t++) {
		vet[t] = t;
	}

	GET_TIME(fim);

  	delta1 = fim - inicio;
  	
  	
	/*int mat1[3][3]={{3,0,2},{9,1,7},{1,0,1}};
	int mat2[3][3]={{1,0,-2},{-2,1,-3},{-1,0,3}};
	int mat3[3][3];
	int M1L=3, M2C=3;
	
	for(linha=0; linha<M1L; linha++){
		for(coluna=0; coluna<M2C; coluna++){
			result=0;
		for(i=0; i<M1L; i++) result+=mat1[linha][i]*mat2[i][coluna];
			mat3[linha][coluna]=result;
    	}
    }

	for(linha=0; linha<M1L; linha++){
		for(coluna=0; coluna<M2C; coluna++)
			printf("%d ", mat3[linha][coluna]);
		printf("\n");
	}*/
	
	printf("Tempo inicializacoes: %.8lf\n", delta1);
	return 0;
  
}
