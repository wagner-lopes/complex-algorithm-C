#include <stdio.h>

typedef struct tamanho{
	int colunas,
		linhas;
} TamanhoTabuleiro;

int*** tabuleiro;

int quantidadeLinhas,
	quantidadeColunas;

int quantidadePosicoes;

void inicializaTabuleiro(int quantidadeTabuleiros){
	int contadorLinhas,
		contadorTabuleiro;

	tabuleiro= (int***) calloc(sizeof(int**),quantidadeTabuleiros);

 	for (contadorTabuleiro = 0; contadorTabuleiro < quantidadeTabuleiros; contadorTabuleiro++){
		tabuleiro[contadorTabuleiro]= (int**) calloc(sizeof(int*),quantidadeLinhas);
		for(contadorLinhas = 0; contadorLinhas < quantidadeLinhas; contadorLinhas++){
				tabuleiro[contadorTabuleiro][contadorLinhas] = (int*)calloc(sizeof(int),quantidadeColunas);
		}
	}
}

