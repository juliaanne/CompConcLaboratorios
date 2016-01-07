#include <stdio.h>
#include <stdlib.h>
#include "timer.h"  //para usar funcoes de medida de tempo
#include <unistd.h> //para usar a funcao "sysconf"
#include <string.h>


void multiplicaMatriz(double ** ){

}




int main(int argc, char *argv[]) {
    double** matriz;
    int linhas,colunas,i,j;
    char nomeDoArquivo[1024];
    FILE* arquivo;

    strcpy(nomeDoArquivo,argv[1]);

    printf("Nome do Arquivo: %s \n",nomeDoArquivo);

    //Lendo o nome do arquivo
    arquivo = fopen(nomeDoArquivo,"r");

    // Testando se o arquivo abriu com sucesso
    if(arquivo == NULL){
        printf("Deu zica no pistão");
        return(0);
    }

    // Lendo o tamanho da Matriz
    fscanf(arquivo,"%d %d",&linhas,&colunas);

    // Alocando espaço para a Matriz
    matriz = (double**) malloc(sizeof(double)*linhas);
    for(i = 0; i < linhas ; i++){
        matriz[i] = (double *) malloc(colunas * sizeof(double));
    }

    printf("Linhas: %d\nColunas: %d \n",linhas, colunas);

    for(i = 0; i < linhas ; i++){
        for(j = 0; j < colunas;j++){
            fscanf(arquivo,"%lf",&matriz[i][j]);
            printf("%lf ",matriz[i][j]);
        }
        printf("\n");
    }




    fclose(arquivo);
    return 0;


}

