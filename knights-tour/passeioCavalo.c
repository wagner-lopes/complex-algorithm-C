#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <time.h>
#include "cavalo.h"

void imprimeCaminho()
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
}

void imprimeCaminhoNaoEncontrado()
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
}

int main(int argc, char **argv)
{
	int linha,
		coluna,
		parametro;

	clock_t inicio, fim, total;

	char *estrategia;

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
		  case 'e':
			estrategia = optarg;
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
					  "\n\t\t-e: Estratégia. Deve aceitar os seguintes valores: ale para aleatório det para determinista"
					  "\n\t\t-s: arquivo de saída\n\n");
			  return 1;
		  default:
			  printf("\n\n\tOs parâmetros aceitos são:");
			  printf ("\n\t\t-n: Número de Linhas "
					  "\n\t\t-m: Número de Colunas"
					  "\n\t\t-i: Posição inicial i"
					  "\n\t\t-j: Posição inicial j"
					  "\n\t\t-e: Estratégia. Deve aceitar os seguintes valores: ale para aleatório det para determinista"
					  "\n\t\t-s: arquivo de saída\n\n");
			  return 1;
		  }
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
		printf("\n\n\tPosicao fora do intervalo do tabuleiro %dx%d!", quantidadeLinhas, quantidadeColunas);
		fprintf(arquivo,"\n\n\tPosicao fora do intervalo do tabuleiro %dx%d!", quantidadeLinhas, quantidadeColunas);
		return (1);
	}

	if (strcmp(estrategia,"ale") && strcmp(estrategia,"det"))
	{
		FILE *arquivo;

		arquivo = fopen (arquivoSaida,"w");
		if (!arquivo)
		{
		  printf ("Erro na abertura do arquivo. Fim de programa.");
		  exit (1);
		}
		printf("\n\n\tEstratégia inválida!");
		fprintf(arquivo,"\n\n\tEstratégia inválida!");
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
	if (!strcmp(estrategia,"det"))
	{
		if(passeioDeterministico(1))
		{
			fim = clock();
			imprimeCaminho();
		}
		else
		{
			fim = clock();
			imprimeCaminhoNaoEncontrado();
		}
	}
	else
	{
		if(passeioAleatorio())
		{
			fim = clock();
			imprimeCaminho();
		}
		else
		{
			fim = clock();
			imprimeCaminhoNaoEncontrado();
		}
	}
	total = fim - inicio;

	printf("\n\t\tTempo de execucao: %0.3lfs", (double)total/CLOCKS_PER_SEC);
	if (!strcmp(estrategia,"ale"))
	{
		printf("\n\t\tTempo da funcao Rand: %0.3lfs", (double)totalRand/CLOCKS_PER_SEC);
	}
	printf("\n\n");
	return 0;
}
