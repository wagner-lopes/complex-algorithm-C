#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <thread_db.h>
#include "tabuleiro.h"

typedef struct posicao{
	int linha,
		coluna;
} Posicao;

typedef struct estruturaMovimentacao{
	int Thread,
		numeroMovimento;
} EstruturaMovimentacao;

typedef struct movimentosValidos{
	int threads;
	int quantidade;
	int movimentos[8];
} EstruturaMovimentos;

Posicao posicaoCavalo[8];
Posicao caminhoPercorrido[8][225];

int quantidadeMovimentosRealizados[8];

/*Variáveis de métricas*/
long unsigned int 	MMovimentosAntesDoRetrocesso = 0,
					MTotalMovimentos = 0;

typedef struct descocamento{
	int linhas,
		colunas;
} Deslocamento;

/*primeiro movimento será descartado*/
Deslocamento movimento[8] = {{ 2, 1},{ 1, 2},{-1, 2},{-2, 1},{-2,-1},{-1,-2},{ 1,-2},{ 2,-1}};

//Variáveis de controle das threads
thread_t identificadorThread[8];
int caminhoEncontrado = -1;
pthread_mutex_t mutex0;
pthread_mutex_t mutex1;
pthread_mutex_t posicao;

int movimentoValido(int identificadorThread, int numeroMovimento){
	Posicao novaPosicao;

	novaPosicao.coluna = posicaoCavalo[identificadorThread].coluna + movimento[numeroMovimento].colunas;
	novaPosicao.linha = posicaoCavalo[identificadorThread].linha + movimento[numeroMovimento].linhas;

	if( novaPosicao.coluna < 0 || novaPosicao.coluna >= quantidadeColunas ||
		novaPosicao.linha  < 0 || novaPosicao.linha  >= quantidadeLinhas){
		return 0;
	}
	else if(tabuleiro[identificadorThread][novaPosicao.linha][novaPosicao.coluna] != 0){
		return 0;
	}
	else{
		return 1;
	}
}

void* movimentaCavalo(void* estrutura){
	Posicao novaPosicao;

	EstruturaMovimentacao* estruturaMovimentacao = (EstruturaMovimentacao*)(estrutura);

	/*Atualiza variáveis de medição*/
	MTotalMovimentos++;

	quantidadeMovimentosRealizados[estruturaMovimentacao->Thread]++;

	novaPosicao.coluna = posicaoCavalo[estruturaMovimentacao->Thread].coluna + movimento[estruturaMovimentacao->numeroMovimento].colunas;
	novaPosicao.linha = posicaoCavalo[estruturaMovimentacao->Thread].linha + movimento[estruturaMovimentacao->numeroMovimento].linhas;

	/*Atualiza caminho*/
	caminhoPercorrido[estruturaMovimentacao->Thread][quantidadeMovimentosRealizados[estruturaMovimentacao->Thread]].coluna = novaPosicao.coluna;
	caminhoPercorrido[estruturaMovimentacao->Thread][quantidadeMovimentosRealizados[estruturaMovimentacao->Thread]].linha = novaPosicao.linha;

	/*Atualiza posição cavalo*/
	posicaoCavalo[estruturaMovimentacao->Thread].linha = novaPosicao.linha;
	posicaoCavalo[estruturaMovimentacao->Thread].coluna = novaPosicao.coluna;

	tabuleiro[estruturaMovimentacao->Thread]
			  [posicaoCavalo[estruturaMovimentacao->Thread].linha]
			  [posicaoCavalo[estruturaMovimentacao->Thread].coluna] =
					  quantidadeMovimentosRealizados[estruturaMovimentacao->Thread]+1;

	return (void*)0;
}

void* desfazMovimento(void* estrutura){
	EstruturaMovimentacao* estruturaMovimentacao = (EstruturaMovimentacao*)(estrutura);

	/*Atualiza variáveis de medição*/
	if(MMovimentosAntesDoRetrocesso == 0){
		MMovimentosAntesDoRetrocesso = quantidadeMovimentosRealizados[estruturaMovimentacao->Thread];
	}
	MTotalMovimentos++;

	tabuleiro[estruturaMovimentacao->Thread]
	          [posicaoCavalo[estruturaMovimentacao->Thread].linha]
	          [posicaoCavalo[estruturaMovimentacao->Thread].coluna] = 0;

	/*Coloca o cavalo na posição anterior*/
	posicaoCavalo[estruturaMovimentacao->Thread].linha =
			caminhoPercorrido[estruturaMovimentacao->Thread][quantidadeMovimentosRealizados[estruturaMovimentacao->Thread]-1].linha;
	posicaoCavalo[estruturaMovimentacao->Thread].coluna =
			caminhoPercorrido[estruturaMovimentacao->Thread][quantidadeMovimentosRealizados[estruturaMovimentacao->Thread]-1].coluna;

	quantidadeMovimentosRealizados[estruturaMovimentacao->Thread]--;

	return (void*)0;
}

int passeio(int thread){
	int numeroMovimento = 0;
	EstruturaMovimentacao estruturaMovimentacao;
	if (caminhoEncontrado != -1) return 0;
	if (quantidadeMovimentosRealizados[thread] >= (quantidadePosicoes-1)){
		caminhoEncontrado = thread;
		return 1;
	}

	estruturaMovimentacao.Thread = thread;

	while (numeroMovimento < 8){
		if(movimentoValido(thread, numeroMovimento)){
			estruturaMovimentacao.numeroMovimento = numeroMovimento;
			movimentaCavalo((void*)(&estruturaMovimentacao));
			if(passeio(thread) != 0) return 1;
		}
		numeroMovimento++;
	}
	desfazMovimento((void*)(&estruturaMovimentacao));
	return 0;
}


void* iniciaPasseio(void* identificadorThread){
	int* thread = identificadorThread;
	passeio(*thread);
	return (void*)0;
}

void* controlaPasseio(void* args){
	int auxiliar;
	EstruturaMovimentacao estruturaMovimentacao;

	EstruturaMovimentos* estrutura = (EstruturaMovimentos*)args;

	for(auxiliar = 0; auxiliar < estrutura->quantidade; auxiliar++){
		if(movimentoValido(estrutura->threads, estrutura->movimentos[auxiliar])){

			estruturaMovimentacao.Thread = estrutura->threads;
			estruturaMovimentacao.numeroMovimento = estrutura->movimentos[auxiliar];

			movimentaCavalo((void*)(&estruturaMovimentacao));

			iniciaPasseio((int*)(&estrutura->threads));
			if (caminhoEncontrado != -1) return (void*)1;
		}
	}
	return 0;
}
