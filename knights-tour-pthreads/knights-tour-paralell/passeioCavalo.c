#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <pthread.h>
#include <thread_db.h>
#include <time.h>
#include "cavalo.h"

int NUM_THREADS = 2;

typedef struct imprime{
	clockid_t total;
	char* arquivoSaida;
} EstruturaImprime;

void* imprimeCaminho(void* estruturaEntrada)
{
	int indiceLinha,
		indiceColuna;
	FILE *arquivo;

	EstruturaImprime* estruturaImprime = (EstruturaImprime*)(estruturaEntrada);

	arquivo = fopen (estruturaImprime->arquivoSaida,"w");
	if (!arquivo)
	    {
	      printf ("Erro na abertura do arquivo. Fim de programa.");
	      exit (1);
	    }
	fprintf(arquivo, "\tSolucao encontrada pela thread %d:\n\n", caminhoEncontrado);
	printf("\tSolucao encontrada pela thread %d:\n\n", caminhoEncontrado);
	for (indiceLinha = 0; indiceLinha < quantidadeLinhas; indiceLinha++)
	{
		fprintf(arquivo, "\t");
		printf("\t");
		for (indiceColuna = 0; indiceColuna < quantidadeColunas; indiceColuna++)
		{
			/*O if abaixo ajusta o espaço entre os números
			 * Se o número for menor que 10 é impresso um espaço a mais
			 * na frente do número para que os outros fiquem alinhados*/
			if(tabuleiro[caminhoEncontrado][indiceLinha][indiceColuna] < 10)
				fprintf(arquivo, "%d   ", tabuleiro[caminhoEncontrado][indiceLinha][indiceColuna]);
			else
				fprintf(arquivo, "%d  ", tabuleiro[caminhoEncontrado][indiceLinha][indiceColuna]);
			printf("\t%d ", tabuleiro[caminhoEncontrado][indiceLinha][indiceColuna]);
		}
		printf("\n");
		fprintf(arquivo, "\n");
	}

	/*Imprime métricas do sistema*/
	printf("\n\n\tMétricas de performance:");
	fprintf(arquivo, "\n\n\tMétricas de performance:");

	printf("\n\t\tTotal de movimentos realizados: %li", MTotalMovimentos);
	printf("\n\t\tQuantidade de movimentos antes do primeiro retrocesso: %li", MMovimentosAntesDoRetrocesso);

	fprintf(arquivo, "\n\t\tTotal de movimentos realizados: %li", MTotalMovimentos);
	fprintf(arquivo, "\n\t\tQuantidade de movimentos antes do primeiro retrocesso: %li", MMovimentosAntesDoRetrocesso);

	printf("\n\t\tTempo de execucao: %0.6lfs\n\n", (double)estruturaImprime->total/CLOCKS_PER_SEC);
	fprintf(arquivo,"\n\t\tTempo de execucao: %0.6lfs\n\n", (double)estruturaImprime->total/CLOCKS_PER_SEC);

	return (void*)0;
}

void* imprimeCaminhoNaoEncontrado(void* estruturaEntrada){
	FILE *arquivo;

	EstruturaImprime* estruturaImprime = (EstruturaImprime*)estruturaEntrada;

	arquivo = fopen (estruturaImprime->arquivoSaida,"w");
	if (!arquivo){
		  printf ("Erro na abertura do arquivo. Fim de programa.");
		  exit (1);
		}

	fprintf(arquivo, "\tNão há solução.");
	printf("\n\tNão há solução");

	printf("\n\t\tTempo de execucao: %0.3lfs\n\n", (double)estruturaImprime->total/CLOCKS_PER_SEC);
	fprintf(arquivo,"\n\t\tTempo de execucao: %0.3lfs\n\n", (double)estruturaImprime->total/CLOCKS_PER_SEC);

	return (void*)0;
}

int main(int argc, char **argv){
	int linha,
		coluna,
		parametro,
		contador;

	clock_t inicio, fim;

	EstruturaImprime estruturaImprime;

	pthread_mutex_init(&mutex0, NULL);
	pthread_mutex_init(&mutex1, NULL);

	while ((parametro = getopt (argc, argv, "m:n:i:j:e:s:")) != -1){
		switch (parametro){
		  case 'n':
			quantidadeLinhas = atoi(optarg);
			break;
		  case 'm':
			quantidadeColunas = atoi(optarg);
			break;
		  case 'i':
			linha = atoi(optarg);
			break;
		  case 'j':
			coluna = atoi(optarg);
			break;
		  case 's':
			estruturaImprime.arquivoSaida = optarg;
			break;
		  case ' ':
			  printf("\n\n\t Os parâmetros aceitos são:");
			  printf ("\n\t\t-n: Número de Linhas "
					  "\n\t\t-m: Número de Colunas"
					  "\n\t\t-i: Posição inicial i"
					  "\n\t\t-j: Posição inicial j"
					  "\n\t\t-s: arquivo de saída\n\n");
			  return 1;
		  default:
			  printf("\n\n\tOs parâmetros aceitos são:");
			  printf ("\n\t\t-n: Número de Linhas "
					  "\n\t\t-m: Número de Colunas"
					  "\n\t\t-i: Posição inicial i"
					  "\n\t\t-j: Posição inicial j"
					  "\n\t\t-s: arquivo de saída\n\n");
			  return 1;
		  }
	}

	if(argc < 11){
		printf("\n\n\tEh necesario informar todos os parametros: ");
		printf ("\n\t\t-n: Número de Linhas "
				"\n\t\t-m: Número de Colunas"
				"\n\t\t-i: Posição inicial i"
				"\n\t\t-j: Posição inicial j"
				"\n\t\t-s: arquivo de saída\n\n");
	}

	if(linha > quantidadeLinhas || coluna > quantidadeColunas){
		FILE *arquivo;

		arquivo = fopen (estruturaImprime.arquivoSaida,"w");
		if (!arquivo){
		  printf ("Erro na abertura do arquivo. Fim de programa.");
		  exit (1);
		}
		printf("\n\n\tPosicao fora do intervalo do tabuleiro %dx%d!\n\n", quantidadeLinhas, quantidadeColunas);
		fprintf(arquivo,"\n\n\tPosicao fora do intervalo do tabuleiro %dx%d!\n\n", quantidadeLinhas, quantidadeColunas);
		return (1);
	}

	inicializaTabuleiro(NUM_THREADS);

	//aloca um vetor que guardará o caminho com o número total de posicoes
	quantidadePosicoes = quantidadeColunas*quantidadeLinhas;

	//ajusta linha e coluna
	linha--;
	coluna--;

	posicaoCavalo[0].coluna = coluna;
	posicaoCavalo[0].linha = linha;

	//Aloca vetores para os caminhos percorridos
	for(contador = 0; contador < NUM_THREADS; contador++){

		//inicializa caminho percorrido
		caminhoPercorrido[contador][0].coluna = coluna;
		caminhoPercorrido[contador][0].linha = linha;

		//marca posicao inicial do tabuleiro
		tabuleiro[contador][linha][coluna] = 1;

		posicaoCavalo[contador].coluna = coluna;
		posicaoCavalo[contador].linha = linha;
		quantidadeMovimentosRealizados[contador] = 0;
	}

	/*Criando estrutura para os movimentos Impares*/
	EstruturaMovimentos estruturaMovimentosPar;
	int contador2 = 0;
	estruturaMovimentosPar.quantidade = 4;
	estruturaMovimentosPar.threads = 0;
	for (contador = 0; contador < 4; contador++){
		estruturaMovimentosPar.movimentos[contador] = contador2;
		contador2+=2;
	}

	/*Criando estrutura para os movimentos Impares*/
	EstruturaMovimentos estruturaMovimentosImpar;
	estruturaMovimentosImpar.quantidade = 4;
	estruturaMovimentosImpar.threads = 1;
	contador2 = 1;
	for (contador = 0; contador < 4; contador++){
		estruturaMovimentosImpar.movimentos[contador] = contador2;
		contador2+=2;
	}

	inicio = clock();

	pthread_create(&(identificadorThread[0]), NULL, controlaPasseio, (void*)(&estruturaMovimentosPar));
	pthread_create(&(identificadorThread[1]), NULL, controlaPasseio, (void*)(&estruturaMovimentosImpar));
	for(contador=0; contador<2; contador++) pthread_join(identificadorThread[contador], NULL);

	fim = clock();

	estruturaImprime.total = fim - inicio;

	if(caminhoEncontrado != -1){
		pthread_create(&(identificadorThread[0]), NULL, imprimeCaminho,(void *)(&estruturaImprime));
	}
	else{
		pthread_create(&(identificadorThread[0]), NULL, imprimeCaminhoNaoEncontrado, (void *)(&estruturaImprime));
	}
	int auxiliar;
	for(auxiliar=0; auxiliar<1; auxiliar++) pthread_join(identificadorThread[0], NULL);
	return 0;
}
