/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Laboratório: 4 */
/* Julia Anne de Souza Alves */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

int nTam, nThreads;
float *vet, *vetResult, *vetSequencial;

void *Somatorio(void *arg)
{
	float somaParcial = 0;
	int i;

	int tid = * (int *) arg;

	//Calculando as somas parciais
	for(i=tid; i<nTam; i+=nThreads){
		somaParcial += vet[i];
	}

	//Alocando resultado no vetor de saída
	vetResult[tid] +=somaParcial;
	
	//Liberando espaços e retornando
	free(arg);
	pthread_exit(NULL);
}


int main(int argc, char *argv[])
{
	int *arg, i, c;
	float somatorio, somatorioThread, somatorioAssociativo, somaParcial;

	FILE *arquivo;
	char nomeArquivo[1000];

	//Validando e recebendo a entrada
	if(argc < 3) {
		printf("Use: %s <arquivo entrada> <numero threads>\n", argv[0]);
		exit(EXIT_FAILURE);
  	}

	//Pegando o nome do arquivo
	strcpy(nomeArquivo, argv[1]);

	//Pegando numero de threads
	nThreads = atoi(argv[2]);

	//Inicializando as threads no sistema
	pthread_t tid_sistema[nThreads];

	//Lendo arquivo
	printf("Nome arquivo: %s \n", nomeArquivo);
	arquivo = fopen(nomeArquivo, "r");

	//Verificando problemas na abertura do arquivo
	if(arquivo == NULL){
		printf("-- Falha na abertura do arquivo \n");
		exit(EXIT_FAILURE);
	}

	//Definindo tamanho do vetor
	fscanf(arquivo, "%d", &nTam);

	//Alocando espaço para o vetor de entrada
	vet = (float*) malloc(sizeof(float) * nTam);

	//Alocando espaço para o vetor de saída
	vetResult = (float*) malloc(sizeof(float) * nThreads);

	//Alocando espaço para o vetor sequencial
	vetSequencial = (float*) malloc(sizeof(float) * nThreads);

	//Verificando problemas na alocação de memória
	if(vet==NULL || vetResult==NULL || vetSequencial==NULL) {
		printf("--ERRO: malloc()\n");
		exit(-1);
	}

	//Inicializando vetor de entrada
	for(i=0; i<nTam; i++){
		fscanf(arquivo, "%f", &vet[i]);
	}

	//Finalizando
	fclose(arquivo);


	//Inicializando as threads
	for(i=0; i<nThreads; i++) {
		printf("--Aloca e preenche argumentos para thread %d\n", i);
		arg = malloc(sizeof(int));

		//Verificando erro na alocação de espaço
	   	if (arg == NULL) {
			printf("--ERRO: malloc()\n"); exit(-1);
    		}
    		*arg = i;
	
    		printf("--Cria a thread %d\n", i+1);

		//Inicializa a execução das threads e verifica erro
    		if (pthread_create(&tid_sistema[i], NULL, Somatorio, (void*) arg)) {
			printf("--ERRO: pthread_create()\n"); exit(-1);
		}
	}

	//Espera as threads terminarem
	for (i=0; i<nThreads; i++) {
		if (pthread_join(tid_sistema[i], NULL)) {
			printf("--ERRO: pthread_join() \n"); exit(-1);
		}
	}

	printf("\n\n");

	//Calculando soma total
	for(i=0; i<nThreads; i++){
		somatorioThread += vetResult[i];
	}
	printf("Terminamos o somatório por threads \nO valor é: %.8f\n\n", somatorioThread);


	//Calculo de forma sequencial
	somatorio=0;
	for(i=0; i<nTam; i++){
		somatorio += vet[i];
	}
	printf("Terminamos o somatório sequencial\nO valor é: %.8f\n", somatorio);

	//Calculo de forma sequencial associativa
	somatorioAssociativo=0;
	for(c=0; c<nThreads; c++){
		somaParcial=0;
		for(i=c; i<nTam; i+=nThreads){
			somaParcial += vet[i];
		}
		vetSequencial[c]=somaParcial;
	}

	for(i=0; i<nThreads; i++){
		somatorioAssociativo+=vetSequencial[i];
	}
	printf("Terminamos o somatório sequencial associativo\nO valor é: %.8f\n", somatorioAssociativo);

	//Liberando espaços
	free(vet);
	free(vetResult);

	return 0;

}
