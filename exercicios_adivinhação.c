#include<stdio.h>
#include<locale.h>
#include <stdlib.h>
#include <time.h>

int main()
{
	setlocale(LC_ALL, "Portuguese");
	srand(time(0));

	int numero_secreto1;
	int numero_secreto2;
	int chute1;
	int chute2;
	int qtd_tentativas;
	int continua = 1;

	while(continua)
	{
		qtd_tentativas = 0;

		numero_secreto1 = (rand()%20)+1;
		numero_secreto2 = (rand()%20)+1;

		do
		{

			printf("O primeiro número secreto é %d \n", numero_secreto1);
			printf("O segundo número secreto é %d \n", numero_secreto2);


			printf("Digite o primeiro número secreto: ");
			scanf("%d", &chute1);

			printf("Digite o segundo número secreto: ");
			scanf("%d", &chute2);

			if(chute1 == numero_secreto1 &&
			        chute2 == numero_secreto2)
			{
				printf("Você acertou os dois números!\n");
			}
			else if (chute1 == numero_secreto1 ||
			         chute2 == numero_secreto2)
				printf("Você acertou um número!\n");


			else

				printf("Você não acertou nenhum número!\n");


			if (chute1 != numero_secreto1 ||
			        chute2 != numero_secreto2);


			qtd_tentativas = qtd_tentativas + 1;



		}
		while (chute1 != numero_secreto1 ||
		        chute2 != numero_secreto2);

		printf("Quantidade de tentativas: %d\n", qtd_tentativas);


		if(qtd_tentativas <= 3)
		{
			printf("Você é ÓTIMO!\n");
			continua = 0;
		}
		else if(qtd_tentativas <= 6)
		{
			printf("Você é BOM!\n");
			continua = 0;

		}
		else if(qtd_tentativas <=10)
		{
			printf("Você é FRACO!\n");

		}
		else
		{
			printf("Você é PÉSSIMO!!!\n");

		}
	}

	return 0;


}
