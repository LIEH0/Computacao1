#include<stdio.h>
#include<locale.h>
#include <stdlib.h>
#include <time.h>

int main()
{
	setlocale(LC_ALL, "Portuguese");
	srand(time(0));

	int numero1;
	int numero2;
	int numero3;
	int chute1;
	int chute2;
	int chute3;
	int qtd_tentativas;
	int continua = 1;
	int dinheiro_total = 0;
	int acertou1;
	int acertou2;
	int acertou3;
	int primeiro_acerto = 1;
	int jogar_novamente;

	while(continua)
	{
		qtd_tentativas = 0;

		numero1 = (rand()%20)+1;


		do {
			numero2 = (rand()%20)+1;
		}

		while(numero1==numero2);

		do
		{
			numero3 = (rand()%20)+1;
		}
		while(numero3==numero1 || numero3==numero2);

		do
		{

			printf("O primeiro número é %d \n", numero1);
			printf("O segundo número é %d \n", numero2);
			printf("O terceiro número é %d \n", numero3);


			printf("Digite o primeiro número: ");
			scanf("%d", &chute1);

			printf("Digite o segundo número: ");
			scanf("%d", &chute2);

			printf("Digite o terceiro número: ");
			scanf("%d", &chute1);

			qtd_tentativas = qtd_tentativas + 1;
			dinheiro_total = dinheiro_total + 1;

			acertou1 = (chute1 == numero1 || chute2 == numero1 || chute3 == numero1 );

			acertou2 = (chute1 == numero2 || chute2 == numero2 || chute3 == numero2 );

			acertou3 = (chute1 == numero3 || chute2 == numero3 || chute3 == numero3 );


			                                 if(acertou1 || acertou2 || acertou3)
			{
				printf("Parabéns! Você acertou");

				primeiro_acerto = 1;

				if(acertou1)
				{
					printf("%d",numero1);
					primeiro_acerto = 0;
				}

				if(acertou2)
				{
					if(primeiro_acerto==0)
						printf(", ");
					printf("%d", numero2);
					primeiro_acerto = 0;
				}

				if(acertou3)
				{
					if(primeiro_acerto==0)
						printf(", ");
					printf("%d",numero3);
				}
				printf("\n");
			}
			else
			{
				printf("Você não acertou nenhum número!\n");
			}

		}

		while(!(acertou1 && acertou2 && acertou3));
		printf("\nVocê acertou os 3 números!\n");
		printf("Quantidade de tentativas: %d\n",qtd_tentativas);
		printf("Dinheiro gasto nas rodadas: R$ %d,00\n",qtd_tentativas);
		if(qtd_tentativas <= 30) {
			printf("Classificação: ÓTIMO!\n");
			continua = 0;
		} else if(qtd_tentativas <= 60) {
			printf("Classificação: BOM!\n");
			continua = 0;
		} else if(qtd_tentativas <= 100) {
			printf("Classificação: FRACO!\n");
		}
		else
		{
			printf("Classificação: PÉSSIMO!\n");

		}
		if(continua == 1) {
			printf("\nDeseja tentar novamente em uma nova rodada?\n");
			printf("Digite 1 para SIM ou 0 para NAO: ");
			scanf("%d", &jogar_novamente);
			if(jogar_novamente == 0) continua = 0;

		}
	}
	printf("\nRESULTADO FINAL\n");
	printf("Dinheiro gasto em todas as rodadas: R$ %d,00", dinheiro_total);

	return 0;
}
