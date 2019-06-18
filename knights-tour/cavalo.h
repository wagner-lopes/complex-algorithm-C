#include <stdio.h>
#include <time.h>
#include "tabuleiro.h"

typedef struct posicao
{
	int linha,
		coluna;
} Posicao;

Posicao posicaoCavalo;
Posicao *caminhoPercorrido;
Posicao ultimoMovimento;
int quantidadeMovimentosRealizados;
char *arquivoSaida;

/*Variáveis de métricas*/
double  MMovimentosAntesDoRetrocesso = 0,
		MTotalMovimentos = 0;

/*Mede tempo da funcao de recurcao*/
clock_t inicioRand,
		fimRand,
		totalRand = 0;

typedef struct descocamento
{
	int linhas,
		colunas;
} Deslocamento;

/*primeiro movimento será descartado*/
Deslocamento movimento[9] = {{0, 0},{ 2, 1},{ 1, 2},{-1, 2},{-2, 1},{-2,-1},{-1,-2},{ 1,-2},{ 2,-1}};

int movimentoValido(int numeroMovimento)
{
	Posicao novaPosicao;

	novaPosicao.coluna = posicaoCavalo.coluna + movimento[numeroMovimento].colunas;
	novaPosicao.linha = posicaoCavalo.linha + movimento[numeroMovimento].linhas;

	if( novaPosicao.coluna < 1 || novaPosicao.coluna > tamanhoTabuleiro.colunas ||
		novaPosicao.linha  < 1 || novaPosicao.linha  > tamanhoTabuleiro.linhas)
	{
		return 0;
	}
	else if(tabuleiro[novaPosicao.linha][novaPosicao.coluna])
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

void movimentaCavalo(int numeroMovimento)
{
	/*Atualiza variáveis de medição*/
	MTotalMovimentos++;

	/*Atualiza caminho*/
	caminhoPercorrido[quantidadeMovimentosRealizados].coluna =
				caminhoPercorrido[quantidadeMovimentosRealizados-1].coluna + movimento[numeroMovimento].colunas;
	caminhoPercorrido[quantidadeMovimentosRealizados].linha =
				caminhoPercorrido[quantidadeMovimentosRealizados-1].linha + movimento[numeroMovimento].linhas;

	/*Atualiza posição cavalo*/
	posicaoCavalo.linha = caminhoPercorrido[quantidadeMovimentosRealizados].linha;
	posicaoCavalo.coluna = caminhoPercorrido[quantidadeMovimentosRealizados].coluna;
}

void desfazMovimento()
{
	/*Atualiza variáveis de medição*/
	if(MMovimentosAntesDoRetrocesso == 0)
	{
		MMovimentosAntesDoRetrocesso = quantidadeMovimentosRealizados;
	}
	MTotalMovimentos++;

	/*Coloca o cavalo na posição anterior*/
	posicaoCavalo.linha = caminhoPercorrido[quantidadeMovimentosRealizados-1].linha;
	posicaoCavalo.coluna = caminhoPercorrido[quantidadeMovimentosRealizados-1].coluna;
}

int passeioDeterministico(int numeroMovimento)
{
	if (quantidadeMovimentosRealizados >= quantidadePosicoes)
	{
		return 1;
	}
	while (numeroMovimento <= 8)
	{
		if(movimentoValido(numeroMovimento))
		{
			quantidadeMovimentosRealizados++;
			movimentaCavalo(numeroMovimento);
			tabuleiro[caminhoPercorrido[quantidadeMovimentosRealizados].linha][caminhoPercorrido[quantidadeMovimentosRealizados].coluna] =
							quantidadeMovimentosRealizados;
			if(passeioDeterministico(1))
			{
				return 1;
			}
		}
		numeroMovimento++;
	}
	desfazMovimento();
	tabuleiro[caminhoPercorrido[quantidadeMovimentosRealizados].linha][caminhoPercorrido[quantidadeMovimentosRealizados].coluna] = 0;
	quantidadeMovimentosRealizados--;
	return 0;
}

int selecionaMovimentoAleatorio(int utilizados[], int quantidadeMovimentos)
{
	int indice = 0,
		disponiveis,
		contador;

	disponiveis = 8 - quantidadeMovimentos;

	inicioRand = clock();

	srand(time(NULL));
	contador = rand()%disponiveis;

	fimRand = clock();
	totalRand += (fimRand - inicioRand);

	while(contador >= 0)
	{
		if(!utilizados[indice])
		{
			contador--;
		}
		indice++;
	}
	return indice;
}

int passeioAleatorio()
{
	int utilizados[8] = {0,0,0,0,0,0,0,0},
		quantidadeMovimentos = 0,
		numeroMovimento;

	if (quantidadeMovimentosRealizados >= quantidadePosicoes)
	{
		return 1;
	}
	while (quantidadeMovimentos < 8)
	{
		numeroMovimento = selecionaMovimentoAleatorio(utilizados, quantidadeMovimentos);
		utilizados[numeroMovimento-1] = 1;
		if(movimentoValido(numeroMovimento))
		{
			quantidadeMovimentosRealizados++;
			movimentaCavalo(numeroMovimento);
			tabuleiro[caminhoPercorrido[quantidadeMovimentosRealizados].linha][caminhoPercorrido[quantidadeMovimentosRealizados].coluna] =
						quantidadeMovimentosRealizados;
			if(passeioAleatorio())
			{
				return(1);
			}
		}
		quantidadeMovimentos++;
	}
	desfazMovimento();
	tabuleiro[caminhoPercorrido[quantidadeMovimentosRealizados].linha][caminhoPercorrido[quantidadeMovimentosRealizados].coluna] = 0;
	quantidadeMovimentosRealizados--;
	return 0;
}
