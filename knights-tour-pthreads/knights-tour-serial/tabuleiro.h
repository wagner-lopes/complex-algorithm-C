#include <stdio.h>

typedef struct tamanho
{
	int colunas,
		linhas;
} TamanhoTabuleiro;

int **tabuleiro;

int quantidadeLinhas,
	quantidadeColunas;

TamanhoTabuleiro tamanhoTabuleiro;

int quantidadePosicoes;

void inicializaTabuleiro(int quantidadeLinhas, int quantidadeColunas)
{
	tabuleiro = (int **) calloc(sizeof(int *),quantidadeLinhas+1);
	int count;
	for (count = 0;count < quantidadeLinhas+1; count++)
	{
		tabuleiro[count] = (int *)calloc(sizeof(int),quantidadeColunas+1);
	}
}
