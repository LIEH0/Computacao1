#include <stdio.h>
#define TAMANHO 5

int no_vetor(int valor, int *vetor, int final)
{
	for (int i = 0; i < final; i=i+1)
	{
		if (vetor[i] == valor)
		{
			return 1;
		}
	}
	return 0;
}

void mostrar_vetor(int *vetor, int tamanho)
{
	printf("\nVetor final preenchido:\n");
	for (int i = 0; i < tamanho; i++)
	{
		printf("vetor[%d] = %d\n", i, vetor[i]);
	}
}


int main()
{
	int valores[TAMANHO];

	printf("Preencha o vetor com %d valores distintos:\n\n", TAMANHO);

	for (int i = 0; i < TAMANHO; i=i+1)
	{
		printf("Digite o valor para a posição %d: ", i);
		scanf("%d", &valores[i]);

		while (no_vetor(valores[i], valores, i))
		{
			printf("Atenção: %d já foi utilizado. Digite um valor distinto.\n", valores[i]);
			printf("Digite novamente o valor para a posição %d: ", i);
			scanf("%d", &valores[i]);
		}
	}

	mostrar_vetor(valores, TAMANHO);

	return 0;
}
