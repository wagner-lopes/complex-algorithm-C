#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <time.h>
#include <pthread.h>
#include "cavalo.h"

int NUM_THREADS = 2;

void imprimeCaminho(clockid_t total, char *arquivoSaida)
{
	int indiceLinha,
		indiceColuna,
		indiceCaminho = 1;
	FILE *arquivo;

	arquivo = fopen (arquivoSaida,"w");
	if (!arquivo)
	    {
	      printf ("Erro na abertura do arquivo. Fim de programa.");
	      exit (1);
	    }
	fprintf(arquivo, "\tSolucao:\n\n");
	printf("\tSolução:\n\n");
	for (indiceLinha = 1; indiceLinha <= quantidadeLinhas; indiceLinha++)
	{
		fprintf(arquivo, "\t");
		printf("\t");
		for (indiceColuna = 1; indiceColuna <= quantidadeColunas; indiceColuna++)
		{
			fprintf(arquivo, "%d  ", tabuleiro[indiceLinha][indiceColuna]);
			printf("\t%d ", tabuleiro[indiceLinha][indiceColuna]);
			indiceCaminho++;
		}
		printf("\n");
		fprintf(arquivo, "\n");
	}

	/*Imprime métricas do sistema*/
	printf("\n\n\tMétricas de performance:");
	fprintf(arquivo, "\n\n\tMétricas de performance:");

	printf("\n\t\tTotal de movimentos realizados: %0.0lf", MTotalMovimentos);
	printf("\n\t\tQuantidade de movimentos antes do primeiro retrocesso: %0.0lf", MMovimentosAntesDoRetrocesso);

	fprintf(arquivo, "\n\t\tTotal de movimentos realizados: %0.0lf", MTotalMovimentos);
	fprintf(arquivo, "\n\t\tQuantidade de movimentos antes do primeiro retrocesso: %0.0lf", MMovimentosAntesDoRetrocesso);

	printf("\n\t\tTempo de execucao: %0.3lfs\n\n", (double)total/CLOCKS_PER_SEC);
	fprintf(arquivo,"\n\t\tTempo de execucao: %0.3lfs\n\n", (double)total/CLOCKS_PER_SEC);
}

void imprimeCaminhoNaoEncontrado(clockid_t total, char *arquivoSaida)
{
	FILE *arquivo;

	arquivo = fopen (arquivoSaida,"w");
	if (!arquivo)
		{
		  printf ("Erro na abertura do arquivo. Fim de programa.");
		  exit (1);
		}

	fprintf(arquivo, "\tNão há solução.");
	printf("\n\tNão há solução");

	printf("\n\t\tTempo de execucao: %0.3lfs\n\n", (double)total/CLOCKS_PER_SEC);
	fprintf(arquivo,"\n\t\tTempo de execucao: %0.3lfs\n\n", (double)total/CLOCKS_PER_SEC);
}

int main(int argc, char **argv)
{
	int linha,
		coluna,
		parametro;
	char *arquivoSaida;

	clock_t inicio, fim, total;

	while ((parametro = getopt (argc, argv, "m:n:i:j:e:s:")) != -1)
	{
		switch (parametro)
		  {
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
			arquivoSaida = optarg;
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

	if(argc < 6)
	{
		printf("\n\n\tEh necesario informar todos os parametros: ");
		printf ("\n\t\t-n: Número de Linhas "
				"\n\t\t-m: Número de Colunas"
				"\n\t\t-i: Posição inicial i"
				"\n\t\t-j: Posição inicial j"
				"\n\t\t-s: arquivo de saída\n\n");
	}

	if(linha > quantidadeLinhas || coluna > quantidadeColunas)
	{
		FILE *arquivo;

		arquivo = fopen (arquivoSaida,"w");
		if (!arquivo)
		{
		  printf ("Erro na abertura do arquivo. Fim de programa.");
		  exit (1);
		}
		printf("\n\n\tPosicao fora do intervalo do tabuleiro %dx%d!\n\n", quantidadeLinhas, quantidadeColunas);
		fprintf(arquivo,"\n\n\tPosicao fora do intervalo do tabuleiro %dx%d!\n\n", quantidadeLinhas, quantidadeColunas);
		return (1);
	}

	inicializaTabuleiro(quantidadeLinhas, quantidadeColunas);
	tamanhoTabuleiro.linhas = quantidadeLinhas;
	tamanhoTabuleiro.colunas = quantidadeColunas;

	//marca posicao inicial do tabuleiro
	tabuleiro[linha][coluna] = 1;

	//aloca um vetor que guardará o caminho com o número total de posicoes
	quantidadePosicoes = quantidadeColunas*quantidadeLinhas;
	caminhoPercorrido = calloc(sizeof(Posicao),quantidadePosicoes+1);

	//inicializa caminho percorrido
	caminhoPercorrido[1].coluna = coluna;
	caminhoPercorrido[1].linha = linha;

	posicaoCavalo.coluna = coluna;
	posicaoCavalo.linha = linha;
	quantidadeMovimentosRealizados = 1;

	inicio = clock();

	int resposta = passeioDeterministico(1);

	if(resposta)
	{
		fim = clock();
		total = fim - inicio;
		imprimeCaminho(total, arquivoSaida);
	}
	else
	{
		fim = clock();
		total = fim - inicio;
		imprimeCaminhoNaoEncontrado(total,arquivoSaida);
	}

	return 0;
}
