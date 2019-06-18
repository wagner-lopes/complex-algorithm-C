/*
 * util.c
 *
 *  Created on: 25/09/2009
 *      Author: wagner
 */

#define _GNU_SOURCE //utilizada na função getline
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct arquivo {
	int quantidadeLinhas;
	int maiorLinha;
} EstruturaArquivo;

typedef struct vetorPesos {
	char* chave;
	int altura;
	int peso;
} EstruturaVetorPesos;

EstruturaArquivo dadosArquivo(char* arquivoEntrada) {
	size_t tamanho = 150;
	char* linha = malloc(tamanho);
	int quantidadeLinhas = 0;
	int tamanhoLinha;
	int maiorLinha = 0;
	EstruturaArquivo EArquivo;

	FILE* arquivo = fopen(arquivoEntrada, "r");
	if (!arquivo) {
		printf("\n\tErro na abertura do arquivo.\n\n");
		exit(1);
	}

	while (getline(&linha, &tamanho, arquivo) > 0) {
		quantidadeLinhas++;
		tamanhoLinha = strlen(linha);
		if (tamanhoLinha > maiorLinha)
			maiorLinha = tamanhoLinha + 1;
	}

	EArquivo.maiorLinha = maiorLinha;
	EArquivo.quantidadeLinhas = quantidadeLinhas;

	fclose(arquivo);
	return EArquivo;
}

char** leArquivo(char* arquivoEntrada, char** vetorTermos) {
	FILE* arquivo = fopen(arquivoEntrada, "r");
	size_t tamanho = 150;
	char* linha = malloc(tamanho);
	int contador = 0;
	int tamanhoUltimaLinha = 0;

	if (!arquivo) {
		printf("\n\tErro na abertura do arquivo.\n\n");
		exit(1);
	}

	while (getline(&linha, &tamanho, arquivo) > 0) {
		strcpy(vetorTermos[contador], linha);
		contador++;
	}
	fclose(arquivo);

	return vetorTermos;
}

char** ordena(int tamanhoVetor, int tamanhoLinha, char** vetor) {
	int contador1, contador2, minimo;
	char* auxiliar;
	auxiliar = malloc(tamanhoLinha);
	for (contador1 = 0; contador1 < tamanhoVetor - 1; contador1++) {
		minimo = contador1;
		for (contador2 = minimo + 1; contador2 < tamanhoVetor; contador2++) {
			if (strcmp(vetor[minimo], vetor[contador2]) > 0) {
				minimo = contador2;
			}
		}
		if (minimo != contador1) {
			strcpy(auxiliar, vetor[contador1]);
			strcpy(vetor[contador1], vetor[minimo]);
			strcpy(vetor[minimo], auxiliar);
		}
	}
	return vetor;
}

