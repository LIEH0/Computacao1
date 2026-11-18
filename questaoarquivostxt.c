#include <stdio.h>
#include <string.h>
#include <locale.h>

FILE *arquivo;
char palavra[100];
char letra;
int contador = 0;

int main() {

	setlocale(LC_ALL, "Portuguese");
	arquivo = fopen("palavras.txt", "w");

	if (arquivo == NULL) {
		printf("Erro ao abrir arquivo!\n");
		return 1;
	}

	do {
		printf("Digite uma palavra (ou 'fim' para encerrar): ");
		scanf("%99s", palavra);

		if (strcmp(palavra, "fim") != 0) {
			fprintf(arquivo, "%s\n", palavra);
		}

	} while (strcmp(palavra, "fim") != 0);

	fclose(arquivo);

	arquivo = fopen("palavras.txt", "r");

	if (arquivo == NULL) {
		printf("Erro ao abrir arquivo para leitura!\n");
		return 1;
	}

	printf("\nDigite a letra para contar as palavras: ");
	scanf(" %c", &letra);


	while (fgets(palavra, 100, arquivo) != NULL) {

		if (palavra[0] == letra) {
			contador++;
		}

	}


	fclose(arquivo);


	printf("Numero de palavras que comecam com '%c' é: %d\n",
	       letra, contador);

	return 0;
}
